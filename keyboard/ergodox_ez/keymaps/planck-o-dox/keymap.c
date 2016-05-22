// qmk
#include "ergodox_ez.h"
#include "debug.h"
#include "action_layer.h"
#include "sleep_led.h"
#include "action.h"

// plugin
#include "plugin.h"

// ultramod
#include "plugins/reset/reset.h"
#include "plugins/ultramod/ultramod.h"
#include "plugins/navigation/navigation.h"

// planck-o-dox
#include "keymap.h"
#include "actions.h"
#include "macros.h"


// Define all keymaps/layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
#include "keymaps/base.c"
,
#include "keymaps/bracket-and-number.c"
,
#include "keymaps/emoticon.c"
,
#include "keymaps/hyper.c"
,
#include "keymaps/cursor-and-fkey.c"
,
#include "keymaps/symbol.c"
};


// Implementation
// Runs just one time when the keyboard initializes.
void
matrix_init_user(void)
{
    // TODO: Once the PluginAddLast() is available, reorder the following to make more sense

    // The default brightness is blinding. Turn it down, please.
    ergodox_led_all_set(LED_BRIGHTNESS_LO);

    // Ultramod plugin
    UltramodConfigureTimeout(ULTRAMOD_TIMEOUT_SINGLE_TAP, 250);
    UltramodConfigureTimeout(ULTRAMOD_TIMEOUT_DOUBLE_TAP, 250);
    UltramodConfigureTimeout(ULTRAMOD_TIMEOUT_ONE_SHOT, 10000);
    UltramodConfigureTimeout(ULTRAMOD_TIMEOUT_LOCKED, 10000);
    Plugin* pUltramodPlugin = UltramodCreatePlugin();
    PluginAddFirst(pUltramodPlugin);

    // Navigation plugin
    Plugin* pNavigationPlugin = NavigationCreatePlugin(CURSOR_AND_FKEY_RIGHT_KEYMAP);
    PluginAddFirst(pNavigationPlugin);

    // Reset plugin
    Plugin* pResetPlugin = ResetCreatePlugin();
    PluginAddFirst(pResetPlugin);
};


// Implementation
// Runs constantly in the background.
void
matrix_scan_user(void)
{
    plugin_matrix_scan();
};


// Kinda hacky to include other definition files in this one, but this avoids mucking with the Makefile
// to compile separately and then link.
#include "macros.c"
#include "actions.c"
#include "plugin.c"
#include "plugins/reset/reset.c"
#include "plugins/ultramod/ultramod.c"
#include "plugins/navigation/navigation.c"
