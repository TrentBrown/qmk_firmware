// system
#include <string.h>

// tmk
#include "action_util.h"
#include "action_code.h"
#include "action.h"
#include "timer.h"
#include "keycode.h"

// ultimod
#include "ultimod.h"


// TODO: Document this. This is a state machine. Use Markdown

// todo:
//      clean up the api for ultimate
//      change filenames from plural to singular



// State machine states
typedef enum UltimodMachineState
{
    NORMAL_STATE,
    HELD_STATE,
    MOMENTARY_STATE,
    ONE_SHOT_STATE,
    LOCKED_STATE
} UltimodMachineState;


// Modifier type
// todo: use these:
typedef enum UltimodModifierType
{
    NO_MODIFIER,
    LAYER_MODIFIER,
    CHARACTER_MODIFIER
} UltimodModifierType;


typedef struct UltimodSettings
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
} UltimodSettings;


// State valid during processing of a single event
typedef struct UltimodEvent
{
    uint16_t code;
    bool pressed;
    bool released;
    keyrecord_t* p_keyrecord;
    uint16_t time;
    bool is_modifier;
    bool is_standard_modifier;
    bool is_layer_modifier;
} UltimodEvent;


// State valid across multiple events
typedef struct UltimodMachine
{
    UltimodMachineState state;
    keypos_t modifier_key;
    uint16_t modifier_pressed_time;
    uint16_t modifier_released_time;
    uint16_t last_action_time;
    uint8_t modifier_bit;
    uint8_t layer;
} UltimodMachine;


// The global Ultimod state
typedef struct Ultimod
{
    UltimodSettings settings;
    UltimodMachine machine;
    UltimodEvent event;
} Ultimod;


// The Ultimod "singleton" (AKA "global")
Ultimod ultimod;


// Forward declarations of local functions
void ultimod_reset(void);
void ultimod_settings_reset(void);
void ultimod_machine_reset(void);
void ultimod_event_reset(void);
uint8_t mod_flag_to_bits(uint8_t flag);
bool is_same_key(keypos_t first, keypos_t second);
bool timed_out(uint16_t first, uint16_t second, uint16_t timeout);
bool character_before_hook(void);
bool modifier_before_hook(void);
bool character_after_hook(void);
bool modifier_after_hook(void);
void ultimod_escape(keyrecord_t* p_keyrecord);
void transition_to_normal_state(void);


void
ultimod_configure_timeout
    (
        const char* p_name,
        uint16_t milliseconds
    )
{
    if (strcmp(p_name, ULTIMOD_TIMEOUT_SINGLE_TAP) == 0)
        ultimod.settings.single_tap_timeout = milliseconds;
    else if (strcmp(p_name, ULTIMOD_TIMEOUT_DOUBLE_TAP) == 0)
        ultimod.settings.double_tap_timeout = milliseconds;
    else if (strcmp(p_name, ULTIMOD_TIMEOUT_ONE_SHOT) == 0)
        ultimod.settings.one_shot_timeout = milliseconds;
    else if (strcmp(p_name, ULTIMOD_TIMEOUT_LOCKED) == 0)
        ultimod.settings.locked_timeout = milliseconds;
}


void
ultimod_reset(void)
{
    clear_keyboard();
    layer_clear();

    ergodox_led_all_off();

    ultimod_settings_reset();
    ultimod_event_reset();
    ultimod_machine_reset();
}


void
ultimod_settings_reset(void)
{
    ultimod.settings.single_tap_timeout = ULTIMOD_TIMEOUT_SINGLE_TAP_DEFAULT;
    ultimod.settings.double_tap_timeout = ULTIMOD_TIMEOUT_DOUBLE_TAP_DEFAULT;
    ultimod.settings.one_shot_timeout = ULTIMOD_TIMEOUT_ONE_SHOT_DEFAULT;
    ultimod.settings.locked_timeout = ULTIMOD_TIMEOUT_LOCKED_DEFAULT;
}


void
ultimod_event_reset(void)
{
    ultimod.event.code = 0;
    ultimod.event.pressed = false;
    ultimod.event.released = false;
    ultimod.event.p_keyrecord  = NULL;
    ultimod.event.time = 0;
    ultimod.event.is_modifier = false;
    ultimod.event.is_standard_modifier = false;
    ultimod.event.is_layer_modifier = false;
}


void
ultimod_machine_reset(void)
{
    ultimod.machine.state = NORMAL_STATE;
    ultimod.machine.modifier_key.col = 0;
    ultimod.machine.modifier_key.row = 0;
    ultimod.machine.modifier_pressed_time = 0;
    ultimod.machine.modifier_released_time = 0;
    ultimod.machine.last_action_time = 0;
    ultimod.machine.modifier_bit = 0;
    ultimod.machine.layer = 0;
}


bool
plugin_process_action_before_hook
    (
        keyrecord_t* p_keyrecord,
        action_t action
    )
{
    bool consumed = false;

    ultimod.event.p_keyrecord = p_keyrecord;

    ultimod.event.pressed = p_keyrecord->event.pressed;
    ultimod.event.released = !ultimod.event.pressed;

    ultimod.event.time = p_keyrecord->event.time;

    if (ultimod.event.pressed)
        ultimod.machine.modifier_pressed_time = ultimod.event.time;
    if (ultimod.event.released)
        ultimod.machine.modifier_released_time = ultimod.event.time;

    ultimod.event.code = action.code;

    ultimod.event.is_standard_modifier = (ultimod.event.code == KC_LSHIFT || ultimod.event.code == KC_RSHIFT || ultimod.event.code == KC_LALT || ultimod.event.code == KC_RALT || ultimod.event.code == KC_LGUI || ultimod.event.code == KC_RGUI || ultimod.event.code == KC_LCTRL || ultimod.event.code == KC_RCTRL);
    if (ultimod.event.is_standard_modifier)
        ultimod.machine.modifier_bit = MOD_BIT(ultimod.event.code);

    ultimod.event.is_layer_modifier = false;
    switch (action.kind.id)
    {
        case ACT_LAYER:
        case ACT_LAYER_TAP:
        case ACT_LAYER_TAP_EXT:
            ultimod.event.is_layer_modifier = true;

            // Yuck:
            keypos_t key = p_keyrecord->event.key;
            uint16_t keycode = keymap_key_to_keycode(layer_switch_get_layer(key), key);
            ultimod.machine.layer = keycode & 0xFF;

            break;
    }

    ultimod.event.is_modifier = (ultimod.event.is_standard_modifier || ultimod.event.is_layer_modifier);

    if (ultimod.event.is_modifier)
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

    if (ultimod.event.is_modifier)
        consumed = modifier_after_hook();
    else
        consumed = character_after_hook(
        );

    return consumed;
}


bool
character_before_hook(void)
{
    bool consumed = false;

    if (ultimod.event.code == KC_ESCAPE)
    {
        ultimod_escape(ultimod.event.p_keyrecord);
        consumed = true;
    }

    return consumed;
}


bool
character_after_hook(void)
{
    const bool consumed = false;

    switch (ultimod.machine.state)
    {
        case NORMAL_STATE:
            break;

        case HELD_STATE:
            if (ultimod.event.pressed)
                ultimod.machine.state = MOMENTARY_STATE;
            break;

        case MOMENTARY_STATE:
            break;

        case ONE_SHOT_STATE:
            if (ultimod.event.pressed)
                transition_to_normal_state();
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
    //    if (ultimod.machine.modifier_bit != get_mods())
    //        return;

    switch (ultimod.machine.state)
    {
        case NORMAL_STATE:
            if (ultimod.event.pressed)
            {
                ultimod.machine.modifier_key = ultimod.event.p_keyrecord->event.key;
                ultimod.machine.state = HELD_STATE;
                consumed = true;
            }
            break;

        case HELD_STATE:
            if (ultimod.event.released)
            {
                if (timed_out(ultimod.event.time, ultimod.machine.modifier_pressed_time, ultimod.settings.single_tap_timeout))
                {
                    transition_to_normal_state();
                }
                else
                {
                    ultimod.machine.last_action_time = ultimod.event.time;
                    ultimod.machine.state = ONE_SHOT_STATE;
                }
                consumed = true;
            }
            break;

        case MOMENTARY_STATE:
            if (ultimod.event.released)
            {
                transition_to_normal_state();
                consumed = true;
            }
            break;

        case ONE_SHOT_STATE:
            if (ultimod.event.pressed)
            {
                if (timed_out(ultimod.event.time, ultimod.machine.modifier_released_time, ultimod.settings.double_tap_timeout))
                    transition_to_normal_state();
                else
                    ultimod.machine.state = LOCKED_STATE;
                consumed = true;
            }
            break;

        case LOCKED_STATE:
            if (ultimod.event.pressed)
            {
                transition_to_normal_state();
                consumed = true;
            }
            break;
    }

    // Not clear to me why we have to to do this
    if (ultimod.machine.state != NORMAL_STATE)
    {
        add_mods(ultimod.machine.modifier_bit);
        layer_on(ultimod.machine.layer);
    }

    return consumed;
}


void ultimod_matrix_scan(void)
{
    const uint16_t now = timer_read();

    switch (ultimod.machine.state)
    {
        case NORMAL_STATE:
            break;

        case HELD_STATE:
            break;

        case MOMENTARY_STATE:
            break;

        case ONE_SHOT_STATE:
            if (timed_out(now, ultimod.machine.last_action_time, ultimod.settings.one_shot_timeout))
                transition_to_normal_state();
            break;

        case LOCKED_STATE:
            if (timed_out(now, ultimod.machine.last_action_time, ultimod.settings.locked_timeout))
                transition_to_normal_state();
            break;
    }

    // Something seems to keep resetting the brightness
    ergodox_led_all_set(LED_BRIGHTNESS_LO);
}


void
ultimod_escape(keyrecord_t* p_keyrecord)
{
    const bool pressed = p_keyrecord->event.pressed;
    if (pressed)
    {
        ultimod_reset();

        add_key(KC_ESCAPE);
        send_keyboard_report();
        del_key(KC_ESCAPE);
        send_keyboard_report();
    }
}


void
transition_to_normal_state(void)
{
    ultimod_reset();
}


void ultimod_set_leds(void)
{
    ergodox_led_all_off();

    switch (ultimod.machine.state)
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


