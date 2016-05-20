// Base
#define BASE_KEYMAP 0
#define BAS_KEYMAP BASE_KEYMAP

// Symbol
#define SYMBOL_KEYMAP 1
#define SYM_KEYMAP SYMBOL_KEYMAP

// Brackets and Numbers
#define BRACKET_AND_NUMBER_KEYMAP 2
#define BAN_KEYMAP BRACKET_AND_NUMBER_KEYMAP

// Function
#define CURSOR_AND_FKEY_LEFT_KEYMAP 3
#define FKEY_KEYMAP CURSOR_AND_FKEY_LEFT_KEYMAP

// Navigation
#define CURSOR_AND_FKEY_RIGHT_KEYMAP 4
#define NAV_KEYMAP CURSOR_AND_FKEY_RIGHT_KEYMAP

// Hyper
#define HYPER_KEYMAP 5
#define HYP_KEYMAP HYPER_KEYMAP

// Emoticon
#define EMOTICON_KEYMAP 6
#define EMO_KEYMAP EMOTICON_KEYMAP


// Define a macro that lets us use more abbreviated reference to a named keymap.
// For example: K(EMO) --> EMO_KEYMAP --> EMOTICON_KEYMAP --> 6
#define K(name) name##_KEYMAP
