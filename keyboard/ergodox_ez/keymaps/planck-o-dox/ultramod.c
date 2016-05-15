// system
#include <string.h>
#include <stdlib.h>

// tmk
#include "action_util.h"
#include "action_code.h"
#include "action.h"
#include "timer.h"
#include "keycode.h"

// plugin
#include "plugin.h"

// ultramod
#include "ultramod.h"


// TODO: Document this. This is a state machine. Use Markdown
// ultramod_(o|o)
// ultramod
// (o|o)

// TODO: Find a way to re-enable one-shot when backspace. Let that feature be an option.

// todo:
//      clean up the api for ultimate
//      change filenames from plural to singular



// State machine states
typedef enum UltramodMachineState
{
    NORMAL_STATE,
    HELD_STATE,
    MOMENTARY_STATE,
    ONE_SHOT_STATE,
    LOCKED_STATE
} UltramodMachineState;


// Modifier type
// todo: use these:
typedef enum UltramodModifier
{
    NO_MODIFIER,
    LAYER_MODIFIER,
    CHARACTER_MODIFIER
} UltramodModifier;


typedef struct UltramodSettings
{
    // Unit for timeouts are milliseconds. A zero value means "never time out".

    // Entering one-shot state
    uint16_t singleTapTimeout;

    // Entering locked state
    uint16_t doubleTapTimeout;

    // Leaving one-shot state if the user hasn't typed any characters
    uint16_t oneShotTimeout;

    // Leaving locked state if the user hasn't typed any characters
    uint16_t lockedTimeout;
} UltramodSettings;


// State valid during processing of a single event
typedef struct UltramodEvent
{
    uint16_t code;
    bool pressed;
    bool released;
    keyrecord_t* pKeyRecord;
    uint16_t time;
    bool isModifier;
    bool isStandardModifier;
    bool isLayerModifier;
} UltramodEvent;


// State valid across multiple events
typedef struct UltramodMachine
{
    UltramodMachineState state;
    keypos_t modifierKey;
    uint16_t modifierPressedTime;
    uint16_t modifierReleasedTime;
    uint16_t lastActionTime;
    uint8_t modifierBit;
    uint8_t layer;
} UltramodMachine;


// The global Ultramod state
typedef struct Ultramod
{
    UltramodSettings settings;
    UltramodMachine machine;
    UltramodEvent event;
} Ultramod;


// The Ultramod "singleton" (ie. our only global variable)
Ultramod ultramod;


// Forward declarations of local functions
void UltramodReset(void);
void UltramodSettingsReset(void);
void UltramodMachineReset(void);
void UltramodEventReset(void);
uint8_t ModFlagToBits(uint8_t flag);
bool IsSameKey(keypos_t first, keypos_t second);
bool TimedOut(uint16_t first, uint16_t second, uint16_t timeout);
bool CharacterBefore(void);
bool ModifierBefore(void);
bool CharacterAfter(void);
bool ModifierAfter(void);
void UltramodEscape(keyrecord_t* pKeyRecord);
void SetNormalState(void);
void UltramodMatrixScan(void);
void UltramodSetLeds(void);


void
UltramodConfigureTimeout
    (
        const char* pName,
        uint16_t milliseconds
    )
{
    if (strcmp(pName, ULTRAMOD_TIMEOUT_SINGLE_TAP) == 0)
        ultramod.settings.singleTapTimeout = milliseconds;
    else if (strcmp(pName, ULTRAMOD_TIMEOUT_DOUBLE_TAP) == 0)
        ultramod.settings.doubleTapTimeout = milliseconds;
    else if (strcmp(pName, ULTRAMOD_TIMEOUT_ONE_SHOT) == 0)
        ultramod.settings.oneShotTimeout = milliseconds;
    else if (strcmp(pName, ULTRAMOD_TIMEOUT_LOCKED) == 0)
        ultramod.settings.lockedTimeout = milliseconds;
}


void
UltramodReset(void)
{
    clear_keyboard();
    layer_clear();

    ergodox_led_all_off();

    UltramodSettingsReset();
    UltramodEventReset();
    UltramodMachineReset();
}


void
UltramodSettingsReset(void)
{
    ultramod.settings.singleTapTimeout = ULTRAMOD_TIMEOUT_SINGLE_TAP_DEFAULT;
    ultramod.settings.doubleTapTimeout = ULTRAMOD_TIMEOUT_DOUBLE_TAP_DEFAULT;
    ultramod.settings.oneShotTimeout = ULTRAMOD_TIMEOUT_ONE_SHOT_DEFAULT;
    ultramod.settings.lockedTimeout = ULTRAMOD_TIMEOUT_LOCKED_DEFAULT;
}


void
UltramodEventReset(void)
{
    ultramod.event.code = 0;
    ultramod.event.pressed = false;
    ultramod.event.released = false;
    ultramod.event.pKeyRecord  = NULL;
    ultramod.event.time = 0;
    ultramod.event.isModifier = false;
    ultramod.event.isStandardModifier = false;
    ultramod.event.isLayerModifier = false;
}


void
UltramodMachineReset(void)
{
    ultramod.machine.state = NORMAL_STATE;
    ultramod.machine.modifierKey.col = 0;
    ultramod.machine.modifierKey.row = 0;
    ultramod.machine.modifierPressedTime = 0;
    ultramod.machine.modifierReleasedTime = 0;
    ultramod.machine.lastActionTime = 0;
    ultramod.machine.modifierBit = 0;
    ultramod.machine.layer = 0;
}


bool
UltramodBefore
    (
        keyrecord_t* pKeyRecord,
        action_t action
    )
{
    bool consumed = false;

    ultramod.event.pKeyRecord = pKeyRecord;

    ultramod.event.pressed = pKeyRecord->event.pressed;
    ultramod.event.released = !ultramod.event.pressed;

    ultramod.event.time = pKeyRecord->event.time;

    if (ultramod.event.pressed)
        ultramod.machine.modifierPressedTime = ultramod.event.time;
    if (ultramod.event.released)
        ultramod.machine.modifierReleasedTime = ultramod.event.time;

    ultramod.event.code = action.code;

    ultramod.event.isStandardModifier = (ultramod.event.code == KC_LSHIFT || ultramod.event.code == KC_RSHIFT || ultramod.event.code == KC_LALT || ultramod.event.code == KC_RALT || ultramod.event.code == KC_LGUI || ultramod.event.code == KC_RGUI || ultramod.event.code == KC_LCTRL || ultramod.event.code == KC_RCTRL);
    if (ultramod.event.isStandardModifier)
        ultramod.machine.modifierBit = MOD_BIT(ultramod.event.code);

    ultramod.event.isLayerModifier = false;
    switch (action.kind.id)
    {
        case ACT_LAYER:
        case ACT_LAYER_TAP:
        case ACT_LAYER_TAP_EXT:
            ultramod.event.isLayerModifier = true;

            // Yuck. A necessary evil.
            keypos_t key = pKeyRecord->event.key;
            uint16_t keycode = keymap_key_to_keycode(layer_switch_get_layer(key), key);
            ultramod.machine.layer = keycode & 0xFF;

            break;
    }

    ultramod.event.isModifier = (ultramod.event.isStandardModifier || ultramod.event.isLayerModifier);

    if (ultramod.event.isModifier)
        consumed = ModifierBefore();
    else
        consumed = CharacterBefore();

    return consumed;
}


bool
UltramodAfter
    (
        keyrecord_t* pKeyRecord,
        action_t action
    )
{
    bool consumed = false;

    if (ultramod.event.isModifier)
        consumed = ModifierAfter();
    else
        consumed = CharacterAfter();

    return consumed;
}


bool
CharacterBefore(void)
{
    bool consumed = false;

    if (ultramod.event.code == KC_ESCAPE)
        UltramodEscape(ultramod.event.pKeyRecord);

    // todo: Why does this state not stick?
    if (ultramod.machine.state != NORMAL_STATE)
    {
        add_mods(ultramod.machine.modifierBit);
        layer_on(ultramod.machine.layer);
    }

    return consumed;
}


bool
CharacterAfter(void)
{
    const bool consumed = false;

    switch (ultramod.machine.state)
    {
        case NORMAL_STATE:
            break;

        case HELD_STATE:

            // The user typing any character commits him to the "momentary" state
            if (ultramod.event.pressed)
                ultramod.machine.state = MOMENTARY_STATE;
            break;

        case MOMENTARY_STATE:
            break;

        case ONE_SHOT_STATE:

            // This character was the one-shot character. Return to normal.
            if (ultramod.event.pressed)
                SetNormalState();
            break;

        case LOCKED_STATE:
            break;
    }

    return consumed;
}


bool
ModifierBefore(void)
{
    bool consumed = false;

    // Make sure modifier chord matches exactly
    //    if (ultramod.machine.modifierBit != get_mods())
    //        return;

    switch (ultramod.machine.state)
    {
        case NORMAL_STATE:
            if (ultramod.event.pressed)
            {
                ultramod.machine.modifierKey = ultramod.event.pKeyRecord->event.key;
                ultramod.machine.state = HELD_STATE;
                consumed = true;
            }
            break;

        case HELD_STATE:
            if (ultramod.event.released)
            {
                const bool tooLateForSingleTap = TimedOut(ultramod.event.time,
                                                          ultramod.machine.modifierPressedTime,
                                                          ultramod.settings.singleTapTimeout);
                if (tooLateForSingleTap)
                {
                    SetNormalState();
                }
                else
                {
                    ultramod.machine.lastActionTime = ultramod.event.time;
                    ultramod.machine.state = ONE_SHOT_STATE;
                }
                consumed = true;
            }
            break;

        case MOMENTARY_STATE:
            if (ultramod.event.released)
            {
                SetNormalState();
                consumed = true;
            }
            break;

        case ONE_SHOT_STATE:
            if (ultramod.event.pressed)
            {
                const bool oneShotTimedOut = TimedOut(ultramod.event.time,
                                                      ultramod.machine.modifierReleasedTime,
                                                      ultramod.settings.doubleTapTimeout);
                if (oneShotTimedOut)
                    SetNormalState();
                else
                    ultramod.machine.state = LOCKED_STATE;
                consumed = true;
            }
            break;

        case LOCKED_STATE:
            if (ultramod.event.pressed)
            {
                SetNormalState();
                consumed = true;
            }
            break;
    }

    // todo: Why does this state not stick?
    if (ultramod.machine.state != NORMAL_STATE)
    {
        add_mods(ultramod.machine.modifierBit);
        layer_on(ultramod.machine.layer);
    }

    return consumed;
}


// TODO: Move some of the following to the "before" so that we can consume the chars. Avoids problem with IntelliJ.
// for example, where double-tap of shift does something.

bool
ModifierAfter(void)
{
    bool consumed = false;

    switch (ultramod.machine.state)
    {
        case NORMAL_STATE:
            break;

        case HELD_STATE:
            break;

        case MOMENTARY_STATE:
            break;

        case ONE_SHOT_STATE:
            break;

        case LOCKED_STATE:
            break;
    }

    // todo: Why does this state not stick?
    if (ultramod.machine.state != NORMAL_STATE)
    {
        add_mods(ultramod.machine.modifierBit);
        layer_on(ultramod.machine.layer);
    }

    return consumed;
}


void UltramodMatrixScan(void)
{
    const uint16_t now = timer_read();

    switch (ultramod.machine.state)
    {
        case NORMAL_STATE:
            break;

        case HELD_STATE:
            break;

        case MOMENTARY_STATE:
            break;

        case ONE_SHOT_STATE:
            {
                const bool oneShotTimedOut = TimedOut(now,
                                                      ultramod.machine.lastActionTime,
                                                      ultramod.settings.oneShotTimeout);
                if (oneShotTimedOut)
                    SetNormalState();
            }
            break;

        case LOCKED_STATE:
            {
                const bool lockTimedOut = TimedOut(now,
                                                   ultramod.machine.lastActionTime,
                                                   ultramod.settings.lockedTimeout);
                if (lockTimedOut)
                    SetNormalState();
            }
            break;
    }

    UltramodSetLeds();
}


void
UltramodEscape(keyrecord_t* pKeyRecord)
{
    if (pKeyRecord->event.pressed)
        SetNormalState();
}


void
SetNormalState(void)
{
    clear_keyboard();
    layer_clear();

    ergodox_led_all_off();

    UltramodEventReset();
    UltramodMachineReset();
}


void UltramodSetLeds(void)
{
    ergodox_led_all_off();

    // todo: Consider changing these so that they progress from left to right, or maybe right to left

    switch (ultramod.machine.state)
    {
        case NORMAL_STATE:
            break;

        case HELD_STATE:
        case MOMENTARY_STATE:
            ergodox_right_led_3_on();  // Blue
            break;

        case ONE_SHOT_STATE:
            ergodox_right_led_2_on();  // Green
            break;

        case LOCKED_STATE:
            ergodox_right_led_1_on();  // Red
            break;
    }

    // Something seems to keep resetting the brightness
    ergodox_led_all_set(LED_BRIGHTNESS_LO);
}


uint8_t
ModFlagToBits(uint8_t flag)
{
    switch (flag)
    {
        case MOD_LSFT:
        case MOD_RSFT:
            return MOD_BIT(KC_LSHIFT);

        case MOD_LCTL:
        case MOD_RCTL:
             return MOD_BIT(KC_LCTRL);

        case MOD_LALT:
        case MOD_RALT:
            return MOD_BIT(KC_LALT);

        case MOD_LGUI:
        case MOD_RGUI:
            return MOD_BIT(KC_LGUI);

        default:
            return MOD_BIT(KC_LSHIFT);  // Whatever
    }
}


bool
TimedOut
    (
        uint16_t first,
        uint16_t second,
        uint16_t timeout
    )
{
    // Zero value means no timeout
    if (timeout == 0)
        return false;

    uint16_t elapsed;
    if (first > second)
        elapsed = first - second;
    else
        elapsed = second - first;
    return (elapsed > timeout);
}


bool
IsSameKey
    (
        keypos_t first,
        keypos_t second
    )
{
    if (first.row != second.row)
        return false;
    if (first.col != second.col)
        return false;
    return true;
}


Plugin*
UltramodCreatePlugin(void)
{
    Plugin* pPlugin = (Plugin*)malloc(sizeof(Plugin));
    pPlugin->pName = "ultramod";
    pPlugin->matrixScan = &UltramodMatrixScan;
    pPlugin->before = &UltramodBefore;
    pPlugin->after = &UltramodAfter;
    pPlugin->pPrevPlugin = NULL;
    pPlugin->pNextPlugin = NULL;
    return pPlugin;
}

