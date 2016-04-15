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


// Configuration
// Unit for timeouts are milliseconds and a zero value means "never time out".
static uint16_t single_tap_timeout = ULTIMOD_TIMEOUT_SINGLE_TAP_DEFAULT;  // Entering one-shot state.
static uint16_t double_tap_timeout = ULTIMOD_TIMEOUT_DOUBLE_TAP_DEFAULT;  // Entering locked state.
static uint16_t one_shot_timeout = ULTIMOD_TIMEOUT_ONE_SHOT_DEFAULT;  // Leaving one-shot state if the user hasn't typed anything.
static uint16_t locked_timeout = ULTIMOD_TIMEOUT_LOCKED_DEFAULT;  // Leaving locked state if the user hasn't typed anything.


// State machine states
#define STATE_NONE 0
#define STATE_HELD 1
#define STATE_MOMENTARY 2
#define STATE_ONE_SHOT 3
#define STATE_LOCKED 4


// Modifier type
#define TYPE_NONE 0
#define TYPE_LAYER 1
#define TYPE_STANDARD 2


// todo: Put these in a fake class (structure)?
static int g_state = STATE_NONE;
static bool g_is_modifier = false;
static bool g_is_standard_modifier = false;
static bool g_is_layer_modifier = false;
static keypos_t g_modifier_key;
static uint16_t g_modifier_pressed_time = 0;
static uint16_t g_modifier_released_time = 0;
static uint16_t g_last_action_time = 0;
static uint8_t g_modifier_bit = 0;
static uint8_t g_layer = 0;
static bool g_pressed = false;
static bool g_released = false;
static uint16_t g_event_time = 0;
static uint16_t g_code = 0;



// Forward declarations of local functions
uint8_t mod_flag_to_bits(uint8_t flag);
bool is_same_key(keypos_t first, keypos_t second);
bool timed_out(uint16_t first, uint16_t second, uint16_t timeout);
void ultimod_clear();
bool character_before_hook
    (
        keyrecord_t* p_record,
        action_t action
    );
bool modifier_before_hook
    (
        keyrecord_t* p_record,
        action_t action
    );
bool character_after_hook
    (
        keyrecord_t* p_record,
        action_t action
    );
bool modifier_after_hook
    (
        keyrecord_t* p_record,
        action_t action
    );
void ultimod_escape(keyrecord_t* p_record);


void
ultimod_configure_timeout
    (
        const char* p_name,
        uint16_t milliseconds
    )
{
    if (strcmp(p_name, ULTIMOD_TIMEOUT_SINGLE_TAP) == 0)
        single_tap_timeout = milliseconds;
    else if (strcmp(p_name, ULTIMOD_TIMEOUT_DOUBLE_TAP) == 0)
        double_tap_timeout = milliseconds;
    else if (strcmp(p_name, ULTIMOD_TIMEOUT_ONE_SHOT) == 0)
        one_shot_timeout = milliseconds;
    else if (strcmp(p_name, ULTIMOD_TIMEOUT_LOCKED) == 0)
        locked_timeout = milliseconds;
}


bool
plugin_process_action_before_hook
    (
        keyrecord_t* p_record,
        action_t action
    )
{
    bool consumed = false;

    g_pressed = p_record->event.pressed;
    g_released = !g_pressed;

    g_event_time = p_record->event.time;

    if (g_pressed)
        g_modifier_pressed_time = g_event_time;
    if (g_released)
        g_modifier_released_time = g_event_time;

    g_code = action.code;

    g_is_standard_modifier = (g_code == KC_LSHIFT || g_code == KC_RSHIFT || g_code == KC_LALT || g_code == KC_RALT || g_code == KC_LGUI || g_code == KC_RGUI || g_code == KC_LCTRL || g_code == KC_RCTRL);
    if (g_is_standard_modifier)
        g_modifier_bit = MOD_BIT(g_code);

    g_is_layer_modifier = false;
    switch (action.kind.id)
    {
        case ACT_LAYER:
        case ACT_LAYER_TAP:
        case ACT_LAYER_TAP_EXT:
            g_is_layer_modifier = true;

            // Yuck:
            keypos_t key = p_record->event.key;
            uint16_t keycode = keymap_key_to_keycode(layer_switch_get_layer(key), key);
            g_layer = keycode & 0xFF;

            break;
    }

    g_is_modifier = (g_is_standard_modifier || g_is_layer_modifier);

    if (g_is_modifier)
        consumed = modifier_before_hook(p_record, action);
    else
        consumed = character_before_hook(p_record, action);

    return consumed;
}


bool
plugin_process_action_after_hook
    (
        keyrecord_t* p_record,
        action_t action
    )
{
    bool consumed = false;

    if (g_is_modifier)
        consumed = modifier_after_hook(p_record, action);
    else
        consumed = character_after_hook(p_record, action);

    return consumed;
}


bool
character_before_hook
    (
        keyrecord_t* p_record,
        action_t action
    )
{
    bool consumed = false;

    if (g_code == KC_ESCAPE)
    {
        ultimod_escape(p_record);
        consumed = true;
    }

    return consumed;
}


bool
character_after_hook
    (
        keyrecord_t* p_record,
        action_t action
    )
{
    const bool consumed = false;

    switch (g_state)
    {
        case STATE_NONE:
            break;

        case STATE_HELD:
            if (g_pressed)
                g_state = STATE_MOMENTARY;
            break;

        case STATE_MOMENTARY:
            break;

        case STATE_ONE_SHOT:
            if (g_pressed)
                transition_to_state_none();
            break;

        case STATE_LOCKED:
            break;
    }

    return consumed;
}


bool
modifier_before_hook
    (
        keyrecord_t* p_record,
        action_t action
    )
{
    return false;
}


// TODO: Move some of the following to the "before" so that we can consume the chars. Avoids problem with IntelliJ.
// for example, where double-tap of shift does something.

bool
modifier_after_hook
    (
        keyrecord_t* p_record,
        action_t action
    )
{
    bool consumed = false;

    // Make sure modifier chord matches exactly
    //    if (g_modifier_bit != get_mods())
    //        return;

    switch (g_state)
    {
        case STATE_NONE:
            if (g_pressed)
            {
                g_modifier_key = p_record->event.key;
                g_state = STATE_HELD;
                consumed = true;
            }
            break;

        case STATE_HELD:
            if (g_released)
            {
                if (timed_out(g_event_time, g_modifier_pressed_time, single_tap_timeout))
                {
                    transition_to_state_none();
                }
                else
                {
                    g_last_action_time = g_event_time;
                    g_state = STATE_ONE_SHOT;
                }
                consumed = true;
            }
            break;

        case STATE_MOMENTARY:
            if (g_released)
            {
                transition_to_state_none();
                consumed = true;
            }
            break;

        case STATE_ONE_SHOT:
            if (g_pressed)
            {
                if (timed_out(g_event_time, g_modifier_released_time, double_tap_timeout))
                    transition_to_state_none();
                else
                    g_state = STATE_LOCKED;
                consumed = true;
            }
            break;

        case STATE_LOCKED:
            if (g_pressed)
            {
                transition_to_state_none();
                consumed = true;
            }
            break;
    }

    // Not clear to me why we have to to do this
    if (g_state != STATE_NONE)
    {
        add_mods(g_modifier_bit);
        layer_on(g_layer);
    }

    return consumed;
}


void ultimod_matrix_scan()
{
    const uint16_t now = timer_read();

    switch (g_state)
    {
        case STATE_NONE:
            break;

        case STATE_HELD:
            break;

        case STATE_MOMENTARY:
            break;

        case STATE_ONE_SHOT:
            if (timed_out(now, g_last_action_time, one_shot_timeout))
                transition_to_state_none();
            break;

        case STATE_LOCKED:
            if (timed_out(now, g_last_action_time, locked_timeout))
                transition_to_state_none();
            break;
    }

    // Something seems to keep resetting the brightness
    ergodox_led_all_set(LED_BRIGHTNESS_LO);
}


void
ultimod_escape(keyrecord_t* p_record)
{
    const bool pressed = p_record->event.pressed;
    if (pressed)
    {
        ultimod_clear();

        add_key(KC_ESCAPE);
        send_keyboard_report();
        del_key(KC_ESCAPE);
        send_keyboard_report();
    }
}


void
transition_to_state_none()
{
    ultimod_clear();
}


void ultimod_set_leds()
{
    ergodox_led_all_off();

    switch (g_state)
    {
        case STATE_NONE:
            break;

        case STATE_HELD:
        case STATE_MOMENTARY:
            ergodox_right_led_2_on();  // Green
            break;

        case STATE_ONE_SHOT:
            ergodox_right_led_3_on();  // Blue
            break;

        case STATE_LOCKED:
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
ultimod_clear()
{
    clear_keyboard();
    layer_clear();

    ergodox_led_all_off();

    g_state = STATE_NONE;
    g_is_modifier = false;
    g_is_standard_modifier = false;
    g_is_layer_modifier = false;
    g_modifier_key.col = 0;
    g_modifier_key.row = 0;
    g_modifier_bit = 0;
    g_modifier_pressed_time = 0;
    g_modifier_released_time = 0;
    g_last_action_time = 0;
    g_layer = 0;
    g_pressed = false;
    g_released = false;
    g_event_time = 0;
    g_code = 0;
}

