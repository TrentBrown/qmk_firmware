// Base
#define BASE_KEYMAP 0
#define BAS_KEYMAP BASE_KEYMAP

// Symbol
#define SYMBOL_KEYMAP 1
#define SYM_KEYMAP SYMBOL_KEYMAP

// Brackets and Numbers
#define BRACKET_AND_NUMBER_KEYMAP 2
#define BAN_KEYMAP BRACKET_AND_NUMBER_KEYMAP

// Cursor and Function
#define NAVIGATION_AND_FUNCTION_KEYMAP 3
#define NAF_KEYMAP NAVIGATION_AND_FUNCTION_KEYMAP

// Hyper
#define HYPER_KEYMAP 4
#define HYP_KEYMAP HYPER_KEYMAP

// Emoticon
#define EMOTICON_KEYMAP 5
#define EMO_KEYMAP EMOTICON_KEYMAP


// Define a macro that lets us use more abbreviated reference to a named keymap.
// For example: K(EMO) --> EMO_KEYMAP --> EMOTICON_KEYMAP --> 5
#define K(name) name##_KEYMAP
