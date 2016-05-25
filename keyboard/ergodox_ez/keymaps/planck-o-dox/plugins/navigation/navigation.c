// plugin
#include "../../plugin.h"

// tmk
#include "action_layer.h"
#include "action_macro.h"

// navigation
#include "navigation.h"


// https://support.apple.com/en-us/HT201236
// http://www.howtogeek.com/115664/42-text-editing-keyboard-shortcuts-that-work-almost-everywhere/


typedef struct NavigationSettings
{
    uint8_t layer;
} NavigationSettings;


typedef enum NavigationDirection
{
    LEFT_DIRECTION,
    LEFT_DIRECTION_WHOLE,

    RIGHT_DIRECTION,
    RIGHT_DIRECTION_WHOLE,

    UP_DIRECTION,

    DOWN_DIRECTION
} NavigationDirection;


typedef enum NavigationUnit
{
    CHAR_UNIT,
    WORD_UNIT,
    LINE_UNIT,
    PARA_UNIT,
    PAGE_UNIT,
    DOC_UNIT,
} NavigationUnit;


typedef enum NavigationAction
{
    MOVE_ACTION,
    SELECT_ACTION,
    DELETE_ACTION,
} NavigationAction;


// State valid across multiple events
typedef struct NavigationMachine
{
    //NavigationMachineState state;
    uint16_t unitCode;
    uint16_t actionCode;
    NavigationUnit unit;
    NavigationAction action;
} NavigationMachine;


// State valid during processing of a single event
typedef struct NavigationEvent
{
    keyrecord_t* pKeyRecord;
    uint16_t code;
    bool pressed;
    bool released;
} NavigationEvent;

// The global Navigation state
typedef struct Navigation
{
    NavigationSettings settings;
    NavigationMachine machine;
    NavigationEvent event;
} Navigation;


// The Navigation "singleton" (ie. our only global variable)
Navigation navigation;


// Forward declarations of local functions

void NavigationReset(void);

bool
NavigationBefore
    (
        keyrecord_t* pKeyRecord,
        action_t action
    );
bool NavigationBeforePerform(void);
const macro_t* pMacro NavigationPerformChar
    (
        NavigationDirection direction,
        NavigationAction action
    );
const macro_t* pMacro NavigationPerformWord
    (
        NavigationDirection direction,
        NavigationAction action
    );
const macro_t* pMacro NavigationPerformLine
    (
        NavigationDirection direction,
        NavigationAction action
    );
const macro_t* pMacro NavigationPerformPara
    (
        NavigationDirection direction,
        NavigationAction action
    );
const macro_t* pMacro NavigationPerformPage
    (
        NavigationDirection direction,
        NavigationAction action
    );
const macro_t* pMacro NavigationPerformDoc
    (
        NavigationDirection direction,
        NavigationAction action
    );

bool NavigationBeforeUnit(void);
bool NavigationBeforeAction(void);


bool
NavigationAfter
    (
        keyrecord_t* pKeyRecord,
        action_t action
    );

void NavigationSetOrClearUnit(NavigationUnit unit);
void NavigationSetOrClearAction(NavigationAction action);

void NavigationPerform(NavigationDirection direction);


Plugin*
NavigationCreatePlugin(uint8_t layer)
{
    NavigationReset();

    navigation.settings.layer = layer;

    Plugin* pPlugin = (Plugin*)malloc(sizeof(Plugin));
    pPlugin->pName = "navigation";
    pPlugin->matrixScan = NULL;
    pPlugin->before = &NavigationBefore;
    pPlugin->after = &NavigationAfter;
    pPlugin->reset = &NavigationReset;
    pPlugin->pPrevPlugin = NULL;
    pPlugin->pNextPlugin = NULL;
    return pPlugin;
}


void
NavigationReset(void)
{
    navigation.machine.unitCode = 0;
    navigation.machine.actionCode = 0;
    navigation.machine.unit = CHAR_UNIT;
    navigation.machine.action = MOVE_ACTION;

    navigation.event.pKeyRecord = NULL;
    navigation.event.code = 0;
    navigation.event.pressed = false;
    navigation.event.released = false;
}


bool
NavigationBefore
    (
        keyrecord_t* pKeyRecord,
        action_t action
    )
{
    uint8_t currentLayer = biton32(layer_state);
    const bool notOurLayer = (currentLayer != navigation.settings.layer);
    if (notOurLayer)
        return false;

    navigation.event.pKeyRecord = pKeyRecord;

    navigation.event.pressed = pKeyRecord->event.pressed;
    navigation.event.released = !navigation.event.pressed;
    navigation.event.code = action.code;

    if (NavigationBeforePerform())
        return true;
    if (NavigationBeforeUnit())
        return true;
    if (NavigationBeforeAction())
        return true;

    return false;
}


bool
NavigationBeforePerform(void)
{
    switch (navigation.event.code)
    {
        case KC_LEFT:
            NavigationPerform(LEFT_DIRECTION);
            return true;
        case KC_H:
            NavigationPerform(LEFT_DIRECTION_WHOLE);
            return true;

        case KC_RIGHT:
            NavigationPerform(RIGHT_DIRECTION);
            return true;
        case KC_K:
            NavigationPerform(RIGHT_DIRECTION_WHOLE);
            return true;

        case KC_UP:
            NavigationPerform(UP_DIRECTION);
            return true;

        case KC_DOWN:
            NavigationPerform(DOWN_DIRECTION);
            return true;
    }
    return false;
}


bool
NavigationBeforeUnit(void)
{
    switch (navigation.event.code)
    {
        case KC_A:
            NavigationSetOrClearUnit(PAGE_UNIT);
            return true;
        case KC_S:
            NavigationSetOrClearUnit(PARA_UNIT);
            return true;
        case KC_D:
            NavigationSetOrClearUnit(LINE_UNIT);
            return true;
        case KC_F:
            NavigationSetOrClearUnit(WORD_UNIT);
            return true;
        case KC_G:
            NavigationSetOrClearUnit(DOC_UNIT);
            return true;
    }
    return false;
}


bool
NavigationBeforeAction(void)
{
    switch (navigation.event.code)
    {
        case KC_LSHIFT:
        case KC_RSHIFT:
        case KC_SPACE:
            NavigationSetOrClearAction(SELECT_ACTION);
            return true;

        case KC_BSPACE:
            NavigationSetOrClearAction(DELETE_ACTION);
            return true;
    }
    return false;
}


void
NavigationSetOrClearUnit(NavigationUnit unit)
{
    if (navigation.event.pressed)
    {
        navigation.machine.unitCode = navigation.event.code;
        navigation.machine.unit = unit;
    }
    else if (navigation.event.released)
    {
        const bool someOtherUnitKeyAlreadyPressed =
            (navigation.machine.unitCode != navigation.event.code);
        if (someOtherUnitKeyAlreadyPressed)
            return;

        navigation.machine.unitCode = 0;
        navigation.machine.unit = CHAR_UNIT;
    }
}


void
NavigationSetOrClearAction(NavigationAction action)
{
    if (navigation.event.pressed)
    {
        navigation.machine.actionCode = navigation.event.code;
        navigation.machine.action = action;
    }
    else if (navigation.event.released)
    {
        const bool otherActionKeyAlreadyPressed =
            (navigation.machine.actionCode != navigation.event.code);
        if (otherActionKeyAlreadyPressed)
            return;

        navigation.machine.actionCode = 0;
        navigation.machine.action = MOVE_ACTION;
    }
}


// Character

#define CHAR_MOVE_LEFT   T(LEFT)
#define CHAR_MOVE_RIGHT  T(RIGHT)
#define CHAR_MOVE_UP     T(UP)
#define CHAR_MOVE_DOWN   T(DOWN)

#define CHAR_SELECT_LEFT   D(LSHIFT), T(LEFT),  U(LSHIFT)
#define CHAR_SELECT_RIGHT  D(LSHIFT), T(RIGHT), U(LSHIFT)
#define CHAR_SELECT_UP     D(LSHIFT), T(UP),    U(LSHIFT)
#define CHAR_SELECT_DOWN   D(LSHIFT), T(DOWN),  U(LSHIFT)


// Word

#define WORD_MOVE_LEFT   D(LALT), T(LEFT),  U(LALT)
#define WORD_MOVE_RIGHT  D(LALT), T(RIGHT), U(LALT)

#define WORD_SELECT_LEFT   D(LSHIFT), D(LALT), T(LEFT),  U(LALT), U(LSHIFT)
#define WORD_SELECT_RIGHT  D(LSHIFT), D(LALT), T(RIGHT), U(LALT), U(LSHIFT)


// Line

#define LINE_MOVE_LEFT   D(LGUI), T(LEFT),  U(LGUI)
#define LINE_MOVE_RIGHT  D(LGUI), T(RIGHT), U(LGUI)
#define LINE_MOVE_UP     T(UP),   D(LGUI),  T(LEFT),  U(LGUI)
#define LINE_MOVE_DOWN   T(DOWN), D(LGUI),  T(RIGHT), U(LGUI)

#define LINE_SELECT_LEFT   D(LSHIFT), D(LGUI), T(LEFT),  U(LGUI), U(LSHIFT)
#define LINE_SELECT_RIGHT  D(LSHIFT), D(LGUI), T(RIGHT), U(LGUI), U(LSHIFT)
#define LINE_SELECT_UP     D(LSHIFT), T(UP),   U(LSHIFT)
#define LINE_SELECT_DOWN   D(LSHIFT), T(DOWN), U(LSHIFT)


// Paragraph

#define PARA_MOVE_LEFT   D(LCTRL), T(A), U(LCTRL)
#define PARA_MOVE_RIGHT  D(LCTRL), T(E), U(LCTRL)

#define PARA_SELECT_LEFT   D(LALT), D(LSHIFT), T(UP),   U(LSHIFT), U(LALT)
#define PARA_SELECT_RIGHT  D(LALT), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT)


// Document

#define DOC_MOVE_LEFT   D(LGUI), T(UP),   U(LGUI)
#define DOC_MOVE_RIGHT  D(LGUI), T(DOWN), U(LGUI)

#define DOC_SELECT_LEFT   D(LSHIFT), D(LGUI), T(UP),   U(LGUI), U(LSHIFT)
#define DOC_SELECT_RIGHT  D(LSHIFT), D(LGUI), T(DOWN), U(LGUI), U(LSHIFT)


void
NavigationPerform(NavigationDirection direction)
{
    if (!navigation.event.pressed)
        return;

    NavigationAction action = navigation.machine.action;

    const macro_t* pMacro = NULL;

    switch (navigation.machine.unit)
    {
        case CHAR_UNIT:
            pMacro = NavigationPerformChar(direction, action);
            break;
        case WORD_UNIT:
            pMacro = NavigationPerformWord(direction, action);
            break;
        case LINE_UNIT:
            pMacro = NavigationPerformLine(direction, action);
            break;
        case PARA_UNIT:
            pMacro = NavigationPerformPara(direction, action);
            break;
        case PAGE_UNIT:
            pMacro = NavigationPerformPage(direction, action);
            break;
        case DOC_UNIT:
            pMacro = NavigationPerformDoc(direction, action);
            break;
    }

    const bool noChars = (pMacro == NULL);
    if (noChars)
        return;

    action_macro_play(pMacro);
}


const macro_t* pMacro
NavigationPerformChar(NavigationDirection direction, NavigationAction action)
{
    switch (direction)
    {
        case LEFT_DIRECTION:
        case LEFT_DIRECTION_WHOLE:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(CHAR_MOVE_LEFT, END);
                case SELECT_ACTION:
                    return MACRO(CHAR_SELECT_LEFT, END);
                case DELETE_ACTION:
                    return MACRO(T(BSPACE), END);
            }
            break;

        case RIGHT_DIRECTION:
        case RIGHT_DIRECTION_WHOLE:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(CHAR_MOVE_RIGHT, END);
                case SELECT_ACTION:
                    return MACRO(CHAR_SELECT_RIGHT, END);
                case DELETE_ACTION:
                    return MACRO(T(DELETE), END);
            }
            break;

        case UP_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(CHAR_MOVE_UP, END);
                case SELECT_ACTION:
                    return MACRO(CHAR_SELECT_UP, END);
                case DELETE_ACTION:
                    return NULL;  // ???
            }
            break;

        case DOWN_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(CHAR_MOVE_DOWN, END);
                case SELECT_ACTION:
                    return MACRO(CHAR_SELECT_DOWN, END);
                case DELETE_ACTION:
                    return NULL;  // ???
            }
            break;
    }
    return NULL;
}


const macro_t* pMacro
NavigationPerformWord(NavigationDirection direction, NavigationAction action)
{
    switch (direction)
    {
        case LEFT_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(WORD_MOVE_LEFT, END);
                case SELECT_ACTION:
                    return MACRO(WORD_SELECT_LEFT, END);
                case DELETE_ACTION:
                    return MACRO(WORD_SELECT_LEFT, T(BSPACE), END);
            }
            break;

        case LEFT_DIRECTION_WHOLE:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(WORD_MOVE_LEFT, END);
                case SELECT_ACTION:
                    return MACRO(WORD_MOVE_RIGHT, WORD_SELECT_LEFT, END);
                case DELETE_ACTION:
                    return MACRO(WORD_MOVE_RIGHT, WORD_SELECT_LEFT, T(BSPACE), END);
            }
            break;

        case RIGHT_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(WORD_MOVE_RIGHT, END);
                case SELECT_ACTION:
                    return MACRO(WORD_SELECT_RIGHT, END);
                case DELETE_ACTION:
                    return MACRO(WORD_SELECT_RIGHT, T(BSPACE), END);
            }
            break;

        case RIGHT_DIRECTION_WHOLE:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(WORD_MOVE_RIGHT, END);
                case SELECT_ACTION:
                    return MACRO(WORD_MOVE_LEFT, WORD_SELECT_RIGHT, END);
                case DELETE_ACTION:
                    return MACRO(WORD_MOVE_LEFT, WORD_SELECT_RIGHT, T(BSPACE), END);
            }
            break;

        case UP_DIRECTION:
            break;

        case DOWN_DIRECTION:
            break;
    }
}

const macro_t* pMacro NavigationPerformLine
    (
        NavigationDirection direction,
        NavigationAction action
    )
{
    switch (direction)
    {
        case LEFT_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(LINE_MOVE_LEFT, END);
                case SELECT_ACTION:
                    return MACRO(LINE_SELECT_LEFT, END);
                case DELETE_ACTION:
                    return MACRO(LINE_SELECT_LEFT, T(BSPACE), END);
            }
            break;

        case LEFT_DIRECTION_WHOLE:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(LINE_MOVE_LEFT, END);
                case SELECT_ACTION:
                    return MACRO(LINE_MOVE_RIGHT, LINE_SELECT_LEFT, END);
                case DELETE_ACTION:
                    return MACRO(LINE_MOVE_RIGHT, LINE_SELECT_LEFT, T(BSPACE), END);
            }
            break;

        case RIGHT_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(LINE_MOVE_RIGHT, END);
                case SELECT_ACTION:
                    return MACRO(LINE_SELECT_RIGHT, END);
                case DELETE_ACTION:
                    return MACRO(LINE_SELECT_RIGHT, T(BSPACE), END);
            }
            break;

        case RIGHT_DIRECTION_WHOLE:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(LINE_MOVE_RIGHT, END);
                case SELECT_ACTION:
                    return MACRO(LINE_MOVE_LEFT, LINE_SELECT_RIGHT, END);
                case DELETE_ACTION:
                    return MACRO(LINE_MOVE_LEFT, LINE_SELECT_RIGHT, T(BSPACE), END);
            }
            break;

        case UP_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(LINE_MOVE_UP, END);
                case SELECT_ACTION:
                    return MACRO(CHAR_SELECT_UP, LINE_SELECT_LEFT, END);
                case DELETE_ACTION:
                    // TODO
            }
            break;

        case DOWN_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(LINE_MOVE_DOWN, END);
                case SELECT_ACTION:
                    return MACRO(CHAR_SELECT_DOWN, LINE_SELECT_RIGHT, END);
                case DELETE_ACTION:
                    // TODO
            }
            break;
    }
}


const macro_t* pMacro NavigationPerformPara
    (
        NavigationDirection direction,
        NavigationAction action
    )
{
    switch (direction)
    {
        case LEFT_DIRECTION:
        case UP_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(PARA_MOVE_LEFT, END);
                case SELECT_ACTION:
                    return MACRO(PARA_SELECT_LEFT, END);
                case DELETE_ACTION:
                    return MACRO(PARA_SELECT_LEFT, T(BSPACE), END);
            }
            break;

        case LEFT_DIRECTION_WHOLE:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(PARA_MOVE_LEFT, END);
                case SELECT_ACTION:
                    return MACRO(PARA_MOVE_RIGHT, PARA_SELECT_LEFT, END);
                case DELETE_ACTION:
                    return MACRO(PARA_MOVE_RIGHT, PARA_SELECT_LEFT, T(BSPACE), END);
            }
            break;

        case RIGHT_DIRECTION:
        case DOWN_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(PARA_MOVE_RIGHT, END);
                case SELECT_ACTION:
                    return MACRO(PARA_SELECT_RIGHT, END);
                case DELETE_ACTION:
                    return MACRO(PARA_SELECT_RIGHT, T(BSPACE), END);
            }
            break;

        case RIGHT_DIRECTION_WHOLE:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(PARA_MOVE_RIGHT, END);
                case SELECT_ACTION:
                    return MACRO(PARA_MOVE_LEFT, PARA_SELECT_RIGHT, END);
                case DELETE_ACTION:
                    return MACRO(PARA_MOVE_LEFT, PARA_SELECT_RIGHT, T(BSPACE), END);
            }
            break;
    }
}


const macro_t* pMacro NavigationPerformPage
    (
        NavigationDirection direction,
        NavigationAction action
    )
{
    // TODO
}


const macro_t* pMacro NavigationPerformDoc
    (
        NavigationDirection direction,
        NavigationAction action
    )
{
    switch (direction)
    {
        case LEFT_DIRECTION:
        case UP_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(DOC_MOVE_LEFT, END);
                case SELECT_ACTION:
                    return MACRO(DOC_SELECT_LEFT, END);
                case DELETE_ACTION:
                    return MACRO(DOC_SELECT_LEFT, T(BSPACE), END);
            }
            break;

        case LEFT_DIRECTION_WHOLE:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(DOC_MOVE_LEFT, END);
                case SELECT_ACTION:
                    return MACRO(DOC_MOVE_RIGHT, DOC_SELECT_LEFT, END);
                case DELETE_ACTION:
                    return MACRO(DOC_MOVE_RIGHT, DOC_SELECT_LEFT, T(BSPACE), END);  // Completely equivalent to RIGHT_DIRECTION_WHOLE
            }
            break;

        case RIGHT_DIRECTION:
        case DOWN_DIRECTION:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(DOC_MOVE_RIGHT, END);
                case SELECT_ACTION:
                    return MACRO(DOC_SELECT_RIGHT, END);
                case DELETE_ACTION:
                    return MACRO(DOC_SELECT_RIGHT, T(BSPACE), END);
            }
            break;

        case RIGHT_DIRECTION_WHOLE:
            switch (action)
            {
                case MOVE_ACTION:
                    return MACRO(DOC_MOVE_RIGHT, END);
                case SELECT_ACTION:
                    return MACRO(DOC_MOVE_LEFT, DOC_SELECT_RIGHT, END);
                case DELETE_ACTION:
                    return MACRO(DOC_MOVE_LEFT, DOC_SELECT_RIGHT, T(BSPACE), END);  // Completely equivalent to LEFT_DIRECTION_WHOLE
            }
            break;
    }
}


bool
NavigationAfter
    (
        keyrecord_t* pKeyRecord,
        action_t action
    )
{
    // TODO: Remove this hook, if it isn't needed

    uint8_t currentLayer = biton32(layer_state);
    const bool notOurLayer = (currentLayer != navigation.settings.layer);
    if (notOurLayer)
        return false;

    bool consumed = false;

    return consumed;
}

