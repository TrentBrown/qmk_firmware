#include "actions.h"
#include "keymap.h"


// Implementation
const uint16_t PROGMEM fn_actions[] =
{
    [SYMBOL_ACTION] = ACTION_LAYER_MOMENTARY(SYMBOL_KEYMAP),
    [BRACKET_AND_NUMBER_ACTION] = ACTION_LAYER_MOMENTARY(BRACKET_AND_NUMBER_KEYMAP),
    [NAVIGATION_ACTION] = ACTION_LAYER_MOMENTARY(NAVIGATION_KEYMAP),
    [FUNCTION_KEY_ACTION] = ACTION_LAYER_MOMENTARY(FUNCTION_KEY_KEYMAP),
    [HYPER_ACTION] = ACTION_LAYER_MOMENTARY(HYPER_KEYMAP),
    [EMOTICON_ACTION] = ACTION_LAYER_MOMENTARY(EMOTICON_KEYMAP)
};
