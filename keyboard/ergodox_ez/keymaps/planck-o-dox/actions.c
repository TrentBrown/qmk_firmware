#include "actions.h"
#include "keymap.h"


// Implementation
const uint16_t PROGMEM fn_actions[] =
{
    [SYMBOL_ACTION] = ACTION_LAYER_MOMENTARY(SYMBOL_KEYMAP),
    [BRACK_AND_NUM_ACTION] = ACTION_LAYER_MOMENTARY(BRACKET_AND_NUMBER_KEYMAP),
    [NAV_AND_FUNC_ACTION] = ACTION_LAYER_MOMENTARY(NAVIGATION_AND_FUNCTION_KEYMAP),
    [HYPER_ACTION] = ACTION_LAYER_MOMENTARY(HYPER_KEYMAP),
    [EMOTICON_ACTION] = ACTION_LAYER_MOMENTARY(EMOTICON_KEYMAP)
};
