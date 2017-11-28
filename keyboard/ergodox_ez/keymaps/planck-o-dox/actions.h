#ifndef __ACTIONS
#define __ACTIONS

// TODO: Shouldn't these start at zero index?

// Keymap/layer switching
#define SYMBOL_ACTION 1
#define SYM_ACTION SYMBOL_ACTION
#define BRACKET_AND_NUMBER_ACTION 2
#define BAN_ACTION BRACKET_AND_NUMBER_ACTION
#define FUNCTION_KEY_ACTION 3
#define FKEY_ACTION FUNCTION_KEY_ACTION
#define NAVIGATION_ACTION 4
#define NAV_ACTION NAVIGATION_ACTION
#define HYPER_ACTION 5
#define HYP_ACTION HYPER_ACTION
#define EMOTICON_ACTION 6
#define EMO_ACTION EMOTICON_ACTION
#define RESET_PLUGINS_ACTION 7
#define RSTP_ACTION RESET_PLUGINS_ACTION


// Other actions
// TODO

// Define a macro that lets us use more abbreviated reference to a named action.
// For example: A(EMO) --> EMO_ACTION  --> EMOTICON_ACTION --> 5
#define A(name) name##_ACTION

#endif //__ACTIONS

