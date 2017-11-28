#include "actions.h"
#include "keymap.h"
#include "plugin.h"

enum function_id {
    RESET_PLUGINS
};

// Implementation
const uint16_t PROGMEM fn_actions[] =
{
    [SYMBOL_ACTION] = ACTION_LAYER_MOMENTARY(SYMBOL_KEYMAP),
    [BRACKET_AND_NUMBER_ACTION] = ACTION_LAYER_MOMENTARY(BRACKET_AND_NUMBER_KEYMAP),
    [NAVIGATION_ACTION] = ACTION_LAYER_MOMENTARY(CURSOR_AND_FKEY_RIGHT_KEYMAP),
    [FUNCTION_KEY_ACTION] = ACTION_LAYER_MOMENTARY(CURSOR_AND_FKEY_LEFT_KEYMAP),
    [HYPER_ACTION] = ACTION_LAYER_MOMENTARY(HYPER_KEYMAP),
    [EMOTICON_ACTION] = ACTION_LAYER_MOMENTARY(EMOTICON_KEYMAP),
    [RESET_PLUGINS_ACTION] = ACTION_FUNCTION(RESET_PLUGINS)
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch (id)
    {
        case RESET_PLUGINS:
            PluginResetAll();
            break;
    }
}
