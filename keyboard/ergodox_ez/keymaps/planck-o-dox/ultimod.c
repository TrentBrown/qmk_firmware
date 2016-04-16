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
    NO_STATE,
    HELD_STATE,
    MOMENTARY_STATE,
    ONE_SHOT_STATE,
    LOCKED_STATE
} UltimodMachineState;


// Modifier type
// todo: use these:
typedef enum UltimodModifierType
{
    TYPE_NONE,
    TYPE_LAYER,
    TYPE_CHARACTER
} UltimodModifierType;


typedef struct UltimodSettings
{
    // Unit for timeouts are milliseconds and a zero value means "never time out".

    // Entering one-shot state.
    uint16_t single_tap_timeout = ULTIMOD_TIMEOUT_SINGLE_TAP_DEFAULT;

    // Entering locked state.
    uint16_t double_tap_timeout = ULTIMOD_TIMEOUT_DOUBLE_TAP_DEFAULT;

    // Leaving one-shot state if the user hasn't typed anything.
    uint16_t one_shot_timeout = ULTIMOD_TIMEOUT_ONE_SHOT_DEFAULT;

    // Leaving locked state if the user hasn't typed anything.
    uint16_t locked_timeout = ULTIMOD_TIMEOUT_LOCKED_DEFAULT;
} UltimodSettings;


// State valid during processing of a single event
typedef struct UltimodEvent
{
    uint16_t code;
    bool pressed;
    bool released;
    keyrecord_t* p_keyrecord;
    uint16_t event_time;
} UltimodEvent;


// State machine state valid across multiple events
typedef struct UltimodMachine
{
    UltimodMachineState state;
    bool is_modifier;
    bool is_standard_modifier;
    bool is_layer_modifier;
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
} UltimodMachine;


// The Ultimod "Singleton" (AKA global)
Ultimod ultimod;


// Forward declarations of local functions
void ultimod_reset();
void ultimod_settings_reset();
void ultimod_machine_reset();
void ultimod_event_reset();
uint8_t mod_flag_to_bits(uint8_t flag);
bool is_same_key(keypos_t first, keypos_t second);
bool timed_out(uint16_t first, uint16_t second, uint16_t timeout);
void ultimod_reset();
bool character_before_hook();
bool modifier_before_hook();
bool character_after_hook();
bool modifier_after_hook();
void ultimod_escape(keyrecord_t* p_keyrecord);


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
ultimod_reset()
{
    ultimod_settings_reset();
    ultimod_event_reset();
    ultimod_machine_reset();
}


void
ultimod_settings_reset()
{
    ultimod.settings.single_tap_timeout = ULTIMOD_TIMEOUT_SINGLE_TAP_DEFAULT;
    ultimod.settings.double_tap_timeout = ULTIMOD_TIMEOUT_DOUBLE_TAP_DEFAULT;
    ultimod.settings.one_shot_timeout = ULTIMOD_TIMEOUT_ONE_SHOT_DEFAULT;
    ultimod.settings.locked_timeout = ULTIMOD_TIMEOUT_LOCKED_DEFAULT;
}


void
ultimod_event_reset()
{
    ultimod.event.code = 0;
    ultimod.event.pressed = false;
    ultimod.event.released = false;
    ultimod.event.p_keyrecord  = NULL;
    ultimod.event.time = 0;
}


void
ultimod_machine_reset()
{
    ultimod.machine.state = NO_STATE;
    ultimod.machine.is_modifier = false;
    ultimod.machine.is_standard_modifier = false;
    ultimod.machine.is_layer_modifier = false;
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

    ultimod.machine.is_standard_modifier = (ultimod.event.code == KC_LSHIFT || ultimod.event.code == KC_RSHIFT || ultimod.event.code == KC_LALT || ultimod.event.code == KC_RALT || ultimod.event.code == KC_LGUI || ultimod.event.code == KC_RGUI || ultimod.event.code == KC_LCTRL || ultimod.event.code == KC_RCTRL);
    if (ultimod.machine.is_standard_modifier)
        ultimod.machine.modifier_bit = MOD_BIT(ultimod.event.code);

    ultimod.machine.is_layer_modifier = false;
    switch (action.kind.id)
    {
        case ACT_LAYER:
        case ACT_LAYER_TAP:
        case ACT_LAYER_TAP_EXT:
            ultimod.machine.is_layer_modifier = true;

            // Yuck:
            keypos_t key = p_keyrecord->event.key;
            uint16_t keycode = keymap_key_to_keycode(layer_switch_get_layer(key), key);
            ultimod.machine.layer = keycode & 0xFF;

            break;
    }

    ultimod.machine.is_modifier = (ultimod.machine.is_standard_modifier || ultimod.machine.is_layer_modifier);

    if (ultimod.machine.is_modifier)
        consumed = modifier_before_hook(&u);
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

    if (ultimod.machine.is_modifier)
        consumed = modifier_after_hook(&u);
    else
        consumed = character_after_hook(&u);

    return consumed;
}


bool
character_before_hook()
{
    bool consumed = false;

    if (ultimod.event.code == KC_ESCAPE)
    {
        ultimod_escape(p_keyrecord);
        consumed = true;
    }

    return consumed;
}


bool
character_after_hook()
{
    const bool consumed = false;

    switch (ultimod.machine.state)
    {
        case NO_STATE:
            break;

        case HELD_STATE:
            if (ultimod.event.pressed)
                ultimod.machine.state = MOMENTARY_STATE;
            break;

        case MOMENTARY_STATE:
            break;

        case ONE_SHOT_STATE:
            if (ultimod.event.pressed)
                transition_to_state_none();
            break;

        case LOCKED_STATE:
            break;
    }

    return consumed;
}


bool
modifier_before_hook()
{
    return false;
}


// TODO: Move some of the following to the "before" so that we can consume the chars. Avoids problem with IntelliJ.
// for example, where double-tap of shift does something.

bool
modifier_after_hook()
{
    bool consumed = false;

    // Make sure modifier chord matches exactly
    //    if (ultimod.machine.modifier_bit != get_mods())
    //        return;

    switch (ultimod.machine.state)
    {
        case NO_STATE:
            if (ultimod.event.pressed)
            {
                ultimod.machine.modifier_key = p_keyrecord->event.key;
                ultimod.machine.state = HELD_STATE;
                consumed = true;
            }
            break;

        case HELD_STATE:
            if (ultimod.event.released)
            {
                if (timed_out(ultimod.event.time, ultimod.machine.modifier_pressed_time, single_tap_timeout))
                {
                    transition_to_state_none();
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
                transition_to_state_none();
                consumed = true;
            }
            break;

        case ONE_SHOT_STATE:
            if (ultimod.event.pressed)
            {
                if (timed_out(ultimod.event.time, ultimod.machine.modifier_released_time, double_tap_timeout))
                    transition_to_state_none();
                else
                    ultimod.machine.state = LOCKED_STATE;
                consumed = true;
            }
            break;

        case LOCKED_STATE:
            if (ultimod.event.pressed)
            {
                transition_to_state_none();
                consumed = true;
            }
            break;
    }

    // Not clear to me why we have to to do this
    if (ultimod.machine.state != NO_STATE)
    {
        add_mods(ultimod.machine.modifier_bit);
        layer_on(ultimod.machine.layer);
    }

    return consumed;
}


void ultimod_matrix_scan()
{
    const uint16_t now = timer_read();

    switch (ultimod.machine.state)
    {
        case NO_STATE:
            break;

        case HELD_STATE:
            break;

        case MOMENTARY_STATE:
            break;

        case ONE_SHOT_STATE:
            if (timed_out(now, ultimod.machine.last_action_time, one_shot_timeout))
                transition_to_state_none();
            break;

        case LOCKED_STATE:
            if (timed_out(now, ultimod.machine.last_action_time, locked_timeout))
                transition_to_state_none();
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
transition_to_state_none()
{
    ultimod_reset();
}


void ultimod_set_leds()
{
    ergodox_led_all_off();

    switch (ultimod.machine.state)
    {
        case NO_STATE:
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


void
ultimod_reset()
{
    clear_keyboard();
    layer_reset();

    ergodox_led_all_off();

    ultimod_machine_reset(m);
}

