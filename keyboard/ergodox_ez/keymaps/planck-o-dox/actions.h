#ifndef __ACTIONS
#define __ACTIONS

// TODO: Shouldn't these start at zero index?

// Keymap/layer switching
#define SYMBOL_ACTION 1
#define SYM_ACTION SYMBOL_ACTION
#define BRACK_AND_NUM_ACTION 2
#define BAN_ACTION BRACK_AND_NUM_ACTION
#define FUNCTION_ACTION 3
#define FUNC_ACTION FUNCTION_ACTION
#define NAVIGATION_ACTION 4
#define NAV_ACTION NAVIGATION_ACTION
#define HYPER_ACTION 5
#define HYP_ACTION HYPER_ACTION
#define EMOTICON_ACTION 6
#define EMO_ACTION EMOTICON_ACTION
#define ESCAPE_ACTION 7
#define UE_ACTION ESCAPE_ACTION

// Other actions
// TODO

// Define a macro that lets us use more abbreviated reference to a named action.
// For example: A(EMO) --> EMO_ACTION  --> EMOTICON_ACTION --> 5
#define A(name) name##_ACTION

#endif //__ACTIONS

