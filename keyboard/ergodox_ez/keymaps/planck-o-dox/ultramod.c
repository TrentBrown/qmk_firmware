// system
#include <string.h>

// tmk
#include "action_util.h"
#include "action_code.h"
#include "action.h"
#include "timer.h"
#include "keycode.h"

// ultramod
#include "ultramod.h"


// TODO: Document this. This is a state machine. Use Markdown
// Ultraman:  (o|o)


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
typedef enum UltramodModifierType
{
    NO_MODIFIER,
    LAYER_MODIFIER,
    CHARACTER_MODIFIER
} UltramodModifierType;


typedef struct UltramodSettings
{
    // Unit for timeouts are milliseconds and a zero value means "never time out"

    // Entering one-shot state
    uint16_t single_tap_timeout;

    // Entering locked state
    uint16_t double_tap_timeout;

    // Leaving one-shot state if the user hasn't typed any characters
    uint16_t one_shot_timeout;

    // Leaving locked state if the user hasn't typed any characters
    uint16_t locked_timeout;
} UltramodSettings;


// State valid during processing of a single event
typedef struct UltramodEvent
{
    uint16_t code;
    bool pressed;
    bool released;
    keyrecord_t* p_keyrecord;
    uint16_t time;
    bool is_modifier;
    bool is_standard_modifier;
    bool is_layer_modifier;
} UltramodEvent;


// State valid across multiple events
typedef struct UltramodMachine
{
    UltramodMachineState state;
    keypos_t modifier_key;
    uint16_t modifier_pressed_time;
    uint16_t modifier_released_time;
    uint16_t last_action_time;
    uint8_t modifier_bit;
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
void ultramod_reset(void);
void ultramod_settings_reset(void);
void ultramod_machine_reset(void);
void ultramod_event_reset(void);
uint8_t mod_flag_to_bits(uint8_t flag);
bool is_same_key(keypos_t first, keypos_t second);
bool timed_out(uint16_t first, uint16_t second, uint16_t timeout);
bool character_before_hook(void);
bool modifier_before_hook(void);
bool character_after_hook(void);
bool modifier_after_hook(void);
void ultramod_escape(keyrecord_t* p_keyrecord);
void set_normal_state(void);


void
ultramod_configure_timeout
    (
        const char* p_name,
        uint16_t milliseconds
    )
{
    if (strcmp(p_name, ULTRAMOD_TIMEOUT_SINGLE_TAP) == 0)
        ultramod.settings.single_tap_timeout = milliseconds;
    else if (strcmp(p_name, ULTRAMOD_TIMEOUT_DOUBLE_TAP) == 0)
        ultramod.settings.double_tap_timeout = milliseconds;
    else if (strcmp(p_name, ULTRAMOD_TIMEOUT_ONE_SHOT) == 0)
        ultramod.settings.one_shot_timeout = milliseconds;
    else if (strcmp(p_name, ULTRAMOD_TIMEOUT_LOCKED) == 0)
        ultramod.settings.locked_timeout = milliseconds;
}


void
ultramod_reset(void)
{
    clear_keyboard();
    layer_clear();

    ergodox_led_all_off();

    ultramod_settings_reset();
    ultramod_event_reset();
    ultramod_machine_reset();
}


void
ultramod_settings_reset(void)
{
    ultramod.settings.single_tap_timeout = ULTRAMOD_TIMEOUT_SINGLE_TAP_DEFAULT;
    ultramod.settings.double_tap_timeout = ULTRAMOD_TIMEOUT_DOUBLE_TAP_DEFAULT;
    ultramod.settings.one_shot_timeout = ULTRAMOD_TIMEOUT_ONE_SHOT_DEFAULT;
    ultramod.settings.locked_timeout = ULTRAMOD_TIMEOUT_LOCKED_DEFAULT;
}


void
ultramod_event_reset(void)
{
    ultramod.event.code = 0;
    ultramod.event.pressed = false;
    ultramod.event.released = false;
    ultramod.event.p_keyrecord  = NULL;
    ultramod.event.time = 0;
    ultramod.event.is_modifier = false;
    ultramod.event.is_standard_modifier = false;
    ultramod.event.is_layer_modifier = false;
}


void
ultramod_machine_reset(void)
{
    ultramod.machine.state = NORMAL_STATE;
    ultramod.machine.modifier_key.col = 0;
    ultramod.machine.modifier_key.row = 0;
    ultramod.machine.modifier_pressed_time = 0;
    ultramod.machine.modifier_released_time = 0;
    ultramod.machine.last_action_time = 0;
    ultramod.machine.modifier_bit = 0;
    ultramod.machine.layer = 0;
}


bool
plugin_process_action_before_hook
    (
        keyrecord_t* p_keyrecord,
        action_t action
    )
{
    bool consumed = false;

    ultramod.event.p_keyrecord = p_keyrecord;

    ultramod.event.pressed = p_keyrecord->event.pressed;
    ultramod.event.released = !ultramod.event.pressed;

    ultramod.event.time = p_keyrecord->event.time;

    if (ultramod.event.pressed)
        ultramod.machine.modifier_pressed_time = ultramod.event.time;
    if (ultramod.event.released)
        ultramod.machine.modifier_released_time = ultramod.event.time;

    ultramod.event.code = action.code;

    ultramod.event.is_standard_modifier = (ultramod.event.code == KC_LSHIFT || ultramod.event.code == KC_RSHIFT || ultramod.event.code == KC_LALT || ultramod.event.code == KC_RALT || ultramod.event.code == KC_LGUI || ultramod.event.code == KC_RGUI || ultramod.event.code == KC_LCTRL || ultramod.event.code == KC_RCTRL);
    if (ultramod.event.is_standard_modifier)
        ultramod.machine.modifier_bit = MOD_BIT(ultramod.event.code);

    ultramod.event.is_layer_modifier = false;
    switch (action.kind.id)
    {
        case ACT_LAYER:
        case ACT_LAYER_TAP:
        case ACT_LAYER_TAP_EXT:
            ultramod.event.is_layer_modifier = true;

            // Yuck. A necessary evil.
            keypos_t key = p_keyrecord->event.key;
            uint16_t keycode = keymap_key_to_keycode(layer_switch_get_layer(key), key);
            ultramod.machine.layer = keycode & 0xFF;

            break;
    }

    ultramod.event.is_modifier = (ultramod.event.is_standard_modifier || ultramod.event.is_layer_modifier);

    if (ultramod.event.is_modifier)
        consumed = modifier_before_hook();
    else
        consumed = character_before_hook();

    return consumed;
}


bool
plugin_process_action_after_hook
    (
        keyrecord_t* p_keyrecord,
        action_t action
    )
{
    bool consumed = false;

    if (ultramod.event.is_modifier)
        consumed = modifier_after_hook();
    else
        consumed = character_after_hook();

    return consumed;
}


bool
character_before_hook(void)
{
    bool consumed = false;

    if (ultramod.event.code == KC_ESCAPE)
    {
        ultramod_escape(ultramod.event.p_keyrecord);
        consumed = true;  // todo: Why not use false here and just piggyback?
    }

    return consumed;
}


bool
character_after_hook(void)
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

            // This character is the one-shot character. Return to normal, but don't consume the character.
            if (ultramod.event.pressed)
                set_normal_state();
            break;

        case LOCKED_STATE:
            break;
    }

    return consumed;
}


bool
modifier_before_hook(void)
{
    return false;
}


// TODO: Move some of the following to the "before" so that we can consume the chars. Avoids problem with IntelliJ.
// for example, where double-tap of shift does something.

bool
modifier_after_hook(void)
{
    bool consumed = false;

    // Make sure modifier chord matches exactly
    //    if (ultramod.machine.modifier_bit != get_mods())
    //        return;

    switch (ultramod.machine.state)
    {
        case NORMAL_STATE:
            if (ultramod.event.pressed)
            {
                ultramod.machine.modifier_key = ultramod.event.p_keyrecord->event.key;
                ultramod.machine.state = HELD_STATE;
                consumed = true;
            }
            break;

        case HELD_STATE:
            if (ultramod.event.released)
            {
                if (timed_out(ultramod.event.time, ultramod.machine.modifier_pressed_time, ultramod.settings.single_tap_timeout))
                {
                    set_normal_state();
                }
                else
                {
                    ultramod.machine.last_action_time = ultramod.event.time;
                    ultramod.machine.state = ONE_SHOT_STATE;
                }
                consumed = true;
            }
            break;

        case MOMENTARY_STATE:
            if (ultramod.event.released)
            {
                set_normal_state();
                consumed = true;
            }
            break;

        case ONE_SHOT_STATE:
            if (ultramod.event.pressed)
            {
                if (timed_out(ultramod.event.time, ultramod.machine.modifier_released_time, ultramod.settings.double_tap_timeout))
                    set_normal_state();
                else
                    ultramod.machine.state = LOCKED_STATE;
                consumed = true;
            }
            break;

        case LOCKED_STATE:
            if (ultramod.event.pressed)
            {
                set_normal_state();
                consumed = true;
            }
            break;
    }

    // Not clear to me why we have to to do this
    if (ultramod.machine.state != NORMAL_STATE)
    {
        add_mods(ultramod.machine.modifier_bit);
        layer_on(ultramod.machine.layer);
    }

    return consumed;
}


void ultramod_matrix_scan(void)
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
            if (timed_out(now, ultramod.machine.last_action_time, ultramod.settings.one_shot_timeout))
                set_normal_state();
            break;

        case LOCKED_STATE:
            if (timed_out(now, ultramod.machine.last_action_time, ultramod.settings.locked_timeout))
                set_normal_state();
            break;
    }

    // Something seems to keep resetting the brightness
    ergodox_led_all_set(LED_BRIGHTNESS_LO);
}


void
ultramod_escape(keyrecord_t* p_keyrecord)
{
    const bool pressed = p_keyrecord->event.pressed;
    if (pressed)
    {
        set_normal_state();

        add_key(KC_ESCAPE);
        send_keyboard_report();
        del_key(KC_ESCAPE);
        send_keyboard_report();
    }
}


void
set_normal_state(void)
{
    clear_keyboard();
    layer_clear();

    ergodox_led_all_off();

    ultramod_event_reset();
    ultramod_machine_reset();
}


void ultramod_set_leds(void)
{
    ergodox_led_all_off();

    switch (ultramod.machine.state)
    {
        case NORMAL_STATE:
            break;

        case HELD_STATE:
        case MOMENTARY_STATE:
            ergodox_right_led_2_on();  // Green
            break;

        case ONE_SHOT_STATE:
            ergodox_right_led_3_on();  // Blue
            break;

        case LOCKED_STATE:
            ergodox_right_led_1_on();  // Red
            break;
    }
}


uint8_t
mod_flag_to_bits(uint8_t flag)
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
timed_out
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
is_same_key
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


