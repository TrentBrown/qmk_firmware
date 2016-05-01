#ifndef __MACROS
#define __MACROS

// Emoticon macros
#define ANGRY_MACRO 0
#define BORED_MACRO 1
#define CRYING_MACRO 2
#define DUNCE_MACRO 3
#define EXTREME_MACRO 4
#define FROWNING_MACRO 5
#define GUILTY_MACRO 6
#define HAPPY_MACRO 7
#define INNOCENT_MACRO 8
#define JUST_MESSED_UP_MACRO 9
#define JUST_MACRO JUST_MESSED_UP_MACRO
#define KISSING_MACRO 10
#define LAUGHING_MACRO 11
#define MEH_MACRO 12
#define N_MACRO 13
#define OH_MACRO 14
#define PERPLEXED_MACRO 15
#define QUIET_MACRO 16
#define ROLLING_EYES_MACRO 17
#define ROLLING_MACRO ROLLING_EYES_MACRO
#define SKEPTICAL_MACRO 18
#define TOUNGUE_MACRO 19
#define UNHAPPY_MACRO 20
#define VEXED_MACRO 21
#define WINKING_MACRO 22
#define X_PARROT_MACRO 23
#define YAY_MACRO 24
#define ZZZ_MACRO 25
#define ZZZ_MACRO 25
#define HEART_MACRO 26

// Bracket macros
#define LCC_MACRO 27
#define RCC_MACRO 28
#define LXC_MACRO 29
#define RXC_MACRO 30

// App macros
#define APP_MACRO 31

// Selection macros
#define WORD_MACRO 32
#define LINE_MACRO 33
#define PARA_MACRO 34
#define PAGE_MACRO 35
#define DOC_MACRO 36


// Define a macro that lets us use more abbreviated reference to a named macro.
// We can't use "M" for our name-expansion macro here because that's already used, and in fact we also want to invoke it.
// So, we use "E" (think: "expand") and do a double expansion using the built-in "M" macro.
// For example: E(HAPPY) --> M(HAPPY_MACRO) --> M(7)
#define E(name) M(name##_MACRO)


#endif //__MACROS

