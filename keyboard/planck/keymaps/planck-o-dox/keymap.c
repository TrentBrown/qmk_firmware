// qmk
#include "planck.h"
#include "debug.h"
#include "action_layer.h"
#include "sleep_led.h"
#include "action.h"

#define PLANCK

// plugin
#include "../../../ergodox_ez/keymaps/planck-o-dox/plugin.h"

// ultramod
#include "../../../ergodox_ez/keymaps/planck-o-dox/plugins/reset/reset.h"
#include "../../../ergodox_ez/keymaps/planck-o-dox/plugins/ultramod/ultramod.h"
#include "../../../ergodox_ez/keymaps/planck-o-dox/plugins/navigation/navigation.h"

// planck-o-dox
#include "../../../ergodox_ez/keymaps/planck-o-dox/keymap.h"
#include "../../../ergodox_ez/keymaps/planck-o-dox/actions.h"
#include "../../../ergodox_ez/keymaps/planck-o-dox/macros.h"


// Define all keymaps/layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
#include "../../../ergodox_ez/keymaps/planck-o-dox/keymaps/base.c"
,
#include "../../../ergodox_ez/keymaps/planck-o-dox/keymaps/bracket-and-number.c"
,
#include "../../../ergodox_ez/keymaps/planck-o-dox/keymaps/emoticon.c"
,
#include "../../../ergodox_ez/keymaps/planck-o-dox/keymaps/hyper.c"
,
#include "../../../ergodox_ez/keymaps/planck-o-dox/keymaps/cursor-and-fkey.c"
,
#include "../../../ergodox_ez/keymaps/planck-o-dox/keymaps/symbol.c"
};


// Implementation
// Runs just one time when the keyboard initializes.
void
matrix_init_user(void)
{
    // TODO: Once the PluginAddLast() is available, reorder the following to make more sense

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
#include "../../../ergodox_ez/keymaps/planck-o-dox/macros.c"
#include "../../../ergodox_ez/keymaps/planck-o-dox/actions.c"
#include "../../../ergodox_ez/keymaps/planck-o-dox/plugin.c"
#include "../../../ergodox_ez/keymaps/planck-o-dox/plugins/reset/reset.c"
#include "../../../ergodox_ez/keymaps/planck-o-dox/plugins/ultramod/ultramod.c"
#include "../../../ergodox_ez/keymaps/planck-o-dox/plugins/navigation/navigation.c"
