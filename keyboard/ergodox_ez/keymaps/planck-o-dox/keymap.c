// qmk
#include "ergodox_ez.h"
#include "debug.h"
#include "action_layer.h"
#include "sleep_led.h"
#include "action.h"

// ultimate
#include "ultimod.h"

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
#include "keymaps/navigation-and-function.c"
,
#include "keymaps/symbol.c"
};


// Implementation
// Runs just one time when the keyboard initializes.
void
matrix_init_user(void)
{
    // The default brightness is blinding. Turn it down, please.
    ergodox_led_all_set(LED_BRIGHTNESS_LO);

    // Set your personal preferences here. See ultimod.h for defaults.
    ultimod_configure_timeout(ULTIMOD_TIMEOUT_SINGLE_TAP, 250);
    ultimod_configure_timeout(ULTIMOD_TIMEOUT_DOUBLE_TAP, 250);
    ultimod_configure_timeout(ULTIMOD_TIMEOUT_ONE_SHOT, 10000);
    ultimod_configure_timeout(ULTIMOD_TIMEOUT_LOCKED, 10000);
};


// Implementation
// Runs constantly in the background.
void
matrix_scan_user(void)
{
    ultimod_matrix_scan();
    ultimod_set_leds();
};


// Kinda hacky to include other definition files in this one, but this avoids mucking with the Makefile
// to compile separately and then link.
#include "macros.c"
#include "actions.c"
#include "ultimod.c"
