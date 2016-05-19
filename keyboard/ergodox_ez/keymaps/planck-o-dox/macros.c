#include "macros.h"

// Forward declarations

const macro_t*
action_get_macro_emoticon
    (
        keyrecord_t* pRecord,
        uint8_t id,
        uint8_t option
    );


// Implementation
const macro_t*
action_get_macro
    (
        keyrecord_t* pRecord,
        uint8_t id,
        uint8_t option
    )
{
    const macro_t* pResult;

    pResult = action_get_macro_emoticon(pRecord, id, option);
    if (pResult != MACRO_NONE)
        return pResult;

    return MACRO_NONE;
};


const macro_t*
action_get_macro_emoticon
    (
        keyrecord_t* pRecord,
        uint8_t id,
        uint8_t option
    )
{
    if (!pRecord->event.pressed)
        return MACRO_NONE;

    switch(id)
    {
        case APP_MACRO:
            return MACRO(I(10), D(LGUI), T(TAB), END);

        case ANGRY_MACRO:
            //   >:(
            return MACRO(I(10), D(LSHIFT), T(DOT), T(SCOLON), T(9), END);

        case BORED_MACRO:
            //   |‑O
            return MACRO(I(10), D(LSHIFT), T(BSLASH), U(LSHIFT), T(MINUS), D(LSHIFT), T(O), U(LSHIFT), END);

        case CRYING_MACRO:
            //   :'‑(
            return MACRO(I(10), D(LSHIFT), T(SCOLON), U(LSHIFT), T(QUOTE), T(MINUS), D(LSHIFT), T(9), U(LSHIFT), END);

        case DUNCE_MACRO:
            //   <:‑I
            return MACRO(I(10), D(LSHIFT), T(COMMA), T(SCOLON), U(LSHIFT), T(MINUS), D(LSHIFT), T(I), U(LSHIFT), END);

        case EXTREME_MACRO:
            //   =:o
            return MACRO(I(10), T(EQUAL), D(LSHIFT), T(SCOLON), U(LSHIFT), T(O), END);

        case FROWNING_MACRO:
            //   :‑[
            return MACRO(I(10), D(LSHIFT), T(SCOLON), U(LSHIFT), T(MINUS), T(LBRACKET), END);

        case GUILTY_MACRO:
            //   }:‑)>
            return MACRO(I(10), D(LSHIFT), T(RBRACKET), T(SCOLON), U(LSHIFT), T(MINUS), D(LSHIFT), T(0), T(DOT), U(LSHIFT), END);

        case HAPPY_MACRO:
            //   :-)
            return MACRO(I(10), D(LSHIFT), T(SCOLON), U(LSHIFT), T(MINUS), D(LSHIFT), T(0), U(LSHIFT), END);

        case INNOCENT_MACRO:
            //   O:‑)
            return MACRO(I(10), D(LSHIFT), T(O), T(SCOLON), U(LSHIFT), T(MINUS), D(LSHIFT), T(0), U(LSHIFT), END);

        case JUST_MESSED_UP_MACRO:
            //    %-|
            return MACRO(I(10), D(LSHIFT), T(5), U(LSHIFT), T(MINUS), D(LSHIFT), T(BSLASH), U(LSHIFT), END);

        case KISSING_MACRO:
            //    :^*
            return MACRO(I(10), D(LSHIFT), T(SCOLON), T(6), T(8), U(LSHIFT), END);

        case LAUGHING_MACRO:
            //   :‑D
            return MACRO(I(10), D(LSHIFT), T(SCOLON), U(LSHIFT), T(MINUS), D(LSHIFT), T(D), U(LSHIFT), END);

        case MEH_MACRO:
            //   :-I
            return MACRO(I(10), D(LSHIFT), T(SCOLON), U(LSHIFT), T(MINUS), D(LSHIFT), T(I), U(LSHIFT), END);

        case N_MACRO:
            //   TBD
            return MACRO(I(10), D(LSHIFT), T(SCOLON), U(LSHIFT), T(MINUS), D(LSHIFT), T(I), U(LSHIFT), END);

        case OH_MACRO:
            //   :-o
            return MACRO(I(10), D(LSHIFT), T(SCOLON), U(LSHIFT), T(MINUS), T(O), END);

        case PERPLEXED_MACRO:
            //   O_o
            return MACRO(I(10), D(LSHIFT), T(O), T(MINUS), U(LSHIFT), T(O), END);

        case QUIET_MACRO:
            //   :-X
            return MACRO(I(10), D(LSHIFT), T(SCOLON), U(LSHIFT), T(MINUS), D(LSHIFT), T(X), U(LSHIFT), END);

        case ROLLING_EYES_MACRO:
            //   9_9
            return MACRO(I(10), T(9), D(LSHIFT), T(MINUS), U(LSHIFT), T(9), END);

        case SKEPTICAL_MACRO:
            //   :‑/
            return MACRO(I(10), D(LSHIFT), T(SCOLON), U(LSHIFT), T(MINUS), T(SLASH), END);

        case TOUNGUE_MACRO:
            //   :‑P
            return MACRO(I(10), D(LSHIFT), T(SCOLON), U(LSHIFT), T(MINUS), D(LSHIFT), T(P), U(LSHIFT), END);

        case UNHAPPY_MACRO:
            //   :-(
            return MACRO(I(10), D(LSHIFT), T(SCOLON), U(LSHIFT), T(MINUS), D(LSHIFT), T(9), U(LSHIFT), END);

        case VEXED_MACRO:
            //   >.<
            return MACRO(I(10), D(LSHIFT), T(DOT), U(LSHIFT), T(DOT), D(LSHIFT), T(COMMA), U(LSHIFT), END);

        case WINKING_MACRO:
            //   ;-)
            return MACRO(I(10), T(SCOLON), U(LSHIFT), T(MINUS), D(LSHIFT), T(0), U(LSHIFT), END);

        case X_PARROT_MACRO:
            //   x_x
            return MACRO(I(10), T(X), D(LSHIFT), T(MINUS), U(LSHIFT), T(X), END);

        case YAY_MACRO:
            //   \o/
            return MACRO(I(10), T(BSLASH), T(O), T(SLASH), END);

        case ZZZ_MACRO:
            //   -_- Zzzzz
            return MACRO(I(10), T(MINUS), D(LSHIFT), T(MINUS), U(LSHIFT), T(MINUS), D(LSHIFT), T(Z), U(LSHIFT), T(Z), T(Z), T(Z), T(Z), END);

        case HEART_MACRO:
            //   <3
            return MACRO(I(10), D(LSHIFT), T(COMMA), U(LSHIFT), T(3), END);

        case LCC_MACRO:
            //   /*
            return MACRO(I(10), T(SLASH), D(LSHIFT), T(8), U(LSHIFT), END);

        case RCC_MACRO:
            //   /*
            return MACRO(I(10), D(LSHIFT), T(8), U(LSHIFT), T(SLASH), END);

        case LXC_MACRO:
            //   <!--
            return MACRO(I(10), D(LSHIFT), T(COMMA), T(1), U(LSHIFT), T(MINUS), T(MINUS), END);

        case RXC_MACRO:
            //   -->
            return MACRO(I(10), T(MINUS), T(MINUS), D(LSHIFT), T(DOT), U(LSHIFT), END);

        case WORD_MACRO:
            //   Select word
            return MACRO(I(10), D(LALT), T(LEFT), D(LSHIFT), T(RIGHT), U(LSHIFT), U(LALT), END);

        case LINE_MACRO:
            //   Select line
            return MACRO(I(10), D(LGUI), T(LEFT), D(LSHIFT), T(RIGHT), U(LSHIFT), U(LGUI), END);

        case PARA_MACRO:
            //   Select paragraph
            return MACRO(I(10), D(LALT), T(UP), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT), END);

        case PAGE_MACRO:
            //   Select page
            return MACRO(I(10), D(LALT), T(HOME), D(LSHIFT), T(END), U(LSHIFT), U(LALT), END);

        case DOC_MACRO:
            //   Select document
            return MACRO(I(10), D(LGUI), T(A), U(LGUI), END);

        case PAMS_MACRO:
            //   Paste and match style (Command-Shift-Option-V)
            return MACRO(I(10), D(LGUI), D(LSHIFT), D(LALT), T(V), U(LALT), U(LSHIFT), U(LGUI), END);
    }
    return MACRO_NONE;
};

#define WORD_MACRO 32
#define LINE_MACRO 33
#define PARA_MACRO 34
