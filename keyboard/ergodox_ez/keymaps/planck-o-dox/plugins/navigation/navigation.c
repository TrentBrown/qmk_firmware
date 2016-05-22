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


typedef enum NavigationVector
{
    LEFT_VECTOR,
    LEFT_WHOLE_VECTOR,

    RIGHT_VECTOR,
    RIGHT_WHOLE_VECTOR,

    UP_VECTOR,

    DOWN_VECTOR
} NavigationVector;


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

void NavigationPerform(NavigationVector vector);


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
            NavigationPerform(LEFT_VECTOR);
            return true;
        case KC_H:
            NavigationPerform(LEFT_WHOLE_VECTOR);
            return true;

        case KC_RIGHT:
            NavigationPerform(RIGHT_VECTOR);
            return true;
        case KC_K:
            NavigationPerform(RIGHT_WHOLE_VECTOR);
            return true;

        case KC_UP:
            NavigationPerform(UP_VECTOR);
            return true;

        case KC_DOWN:
            NavigationPerform(DOWN_VECTOR);
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


// TODO: Factor out parts of this function. Is too big.
// TODO: Try to find a way to factor out common macro sequences. Is there an "append"?

#define WORD_MOVE_LEFT T(LEFT)
#define WORD_MOVE_RIGHT T(RIGHT)
#define WORD_MOVE_UP T(UP)
#define WORD_MOVE_DOWN T(DOWN)

#define WORD_SELECT_LEFT D(LSHIFT), T(LEFT), U(LSHIFT)
#define WORD_SELECT_RIGHT D(LSHIFT), T(RIGHT), U(LSHIFT)
#define WORD_SELECT_UP D(LSHIFT), T(UP), U(LSHIFT)
#define WORD_SELECT_DOWN D(LSHIFT), T(DOWN), U(LSHIFT)

void
NavigationPerform(NavigationVector vector)
{
    if (!navigation.event.pressed)
        return;

    NavigationAction action = navigation.machine.action;

    const macro_t* pMacro = NULL;

    switch (navigation.machine.unit)
    {
        case CHAR_UNIT:

            switch (vector)
            {
                case LEFT_VECTOR:
                case LEFT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), T(LEFT), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), T(LEFT), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), T(BSPACE), END);
                            break;
                    }
                    break;

                case RIGHT_VECTOR:
                case RIGHT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), T(RIGHT), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), T(RIGHT), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), T(DELETE), END);
                            break;
                    }
                    break;

                case UP_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), T(UP), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), T(UP), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            break;
                    }
                    break;

                case DOWN_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), T(DOWN), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), T(DOWN), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            break;
                    }
                    break;
            }
            break;

        case WORD_UNIT:
            switch (vector)
            {
                case LEFT_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LALT), T(LEFT), U(LALT), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LALT), T(LEFT), U(LALT), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LALT), T(LEFT), U(LALT), U(LSHIFT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case LEFT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LALT), T(LEFT), U(LALT), END);  // Same as non-whole
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LALT), T(RIGHT), U(LALT),   D(LSHIFT), D(LALT), T(LEFT), U(LALT), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LALT), T(RIGHT), U(LALT),   D(LSHIFT), D(LALT), T(LEFT), U(LALT), U(LSHIFT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case RIGHT_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LALT), T(RIGHT), U(LALT), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LALT), T(RIGHT), U(LALT), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LALT), T(RIGHT), U(LALT), U(LSHIFT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case RIGHT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LALT), T(RIGHT), U(LALT), END);  // Same as non-whole
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LALT), T(RIGHT), U(LALT),   D(LSHIFT), D(LALT), T(RIGHT), U(LALT), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LALT), T(RIGHT), U(LALT),   D(LSHIFT), D(LALT), T(RIGHT), U(LALT), U(LSHIFT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case UP_VECTOR:
                    break;

                case DOWN_VECTOR:
                    break;
            }
            break;

        case LINE_UNIT:
            switch (vector)
            {
                case LEFT_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(LEFT), U(LGUI), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(LEFT), U(LGUI), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(LEFT), U(LGUI), U(LSHIFT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case LEFT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(LEFT), U(LGUI), END);  // Same as non-whole
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(RIGHT), U(LGUI),   D(LSHIFT), D(LGUI), T(LEFT), U(LGUI), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(RIGHT), U(LGUI),   D(LSHIFT), D(LGUI), T(LEFT), U(LGUI), U(LSHIFT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case RIGHT_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(RIGHT), U(LGUI), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(RIGHT), U(LGUI), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(RIGHT), U(LGUI), U(LSHIFT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case RIGHT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(RIGHT), U(LGUI), END);  // Same as non-whole
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(LEFT), U(LGUI),   D(LSHIFT), D(LGUI), T(RIGHT), U(LGUI), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(LEFT), U(LGUI),   D(LSHIFT), D(LGUI), T(RIGHT), U(LGUI), U(LSHIFT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case UP_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), T(UP),   D(LGUI), T(LEFT), U(LGUI),   END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), T(UP), U(LSHIFT),   D(LSHIFT), D(LGUI), T(LEFT), U(LGUI), U(LSHIFT),   END);
                            break;
                        case DELETE_ACTION:
                            break;
                    }
                    break;

                case DOWN_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), T(DOWN),   D(LGUI), T(RIGHT), U(LGUI),   END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), T(DOWN), U(LSHIFT),   D(LSHIFT), D(LGUI), T(RIGHT), U(LGUI), U(LSHIFT),   END);
                            break;
                        case DELETE_ACTION:
                            break;
                    }
                    break;
            }
            break;

        case PARA_UNIT:
            switch (vector)
            {
                case LEFT_VECTOR:
                case UP_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(A), U(LCTRL), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LALT), D(LSHIFT), T(UP), U(LSHIFT), U(LALT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LALT), D(LSHIFT), T(UP), U(LSHIFT), U(LALT),   T(BSPACE),   END);;
                            break;
                    }
                    break;

                case LEFT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(A), U(LCTRL), END);  // Same as non-whole
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(E), U(LCTRL),   D(LALT), D(LSHIFT), T(UP), U(LSHIFT), U(LALT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(E), U(LCTRL),   D(LALT), D(LSHIFT), T(UP), U(LSHIFT),   T(BSPACE),   END);;
                            break;
                    }
                    break;

                case RIGHT_VECTOR:
                case DOWN_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(E), U(LCTRL), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LALT), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LALT), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case RIGHT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(E), U(LCTRL), END);  // Same as non-whole
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(A), U(LCTRL),   D(LALT), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(A), U(LCTRL),   D(LALT), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT),   T(BSPACE),   END);
                            break;
                    }
                    break;
            }
            break;

        case PAGE_UNIT:
            switch (vector)
            {
                case LEFT_VECTOR:
                case UP_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(A), U(LCTRL), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LALT), D(LSHIFT), T(UP), U(LSHIFT), U(LALT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LALT), D(LSHIFT), T(UP), U(LSHIFT), U(LALT),   T(BSPACE),   END);;
                            break;
                    }
                    break;

                case LEFT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(A), U(LCTRL), END);  // Same as non-whole
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(E), U(LCTRL),   D(LALT), D(LSHIFT), T(UP), U(LSHIFT), U(LALT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(E), U(LCTRL),   D(LALT), D(LSHIFT), T(UP), U(LSHIFT),   T(BSPACE),   END);;
                            break;
                    }
                    break;

                case RIGHT_VECTOR:
                case DOWN_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(E), U(LCTRL), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LALT), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LALT), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case RIGHT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(E), U(LCTRL), END);  // Same as non-whole
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(A), U(LCTRL),   D(LALT), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(A), U(LCTRL),   D(LALT), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT),   T(BSPACE),   END);
                            break;
                    }
                    break;
            }
            break;

        case DOC_UNIT:
            switch (vector)
            {
                case LEFT_VECTOR:
                case UP_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(UP), U(LGUI), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(UP), U(LGUI), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(UP), U(LGUI), U(LSHIFT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case LEFT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(UP), U(LGUI), END);  // Same as non-whole
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(DOWN), U(LGUI),   D(LSHIFT), D(LGUI), T(UP), U(LGUI), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(DOWN), U(LGUI),   D(LSHIFT), D(LGUI), T(UP), U(LGUI), U(LSHIFT),   T(BSPACE),   END);  // Completely equivalent to RIGHT_WHOLE_VECTOR
                            break;
                    }
                    break;

                case RIGHT_VECTOR:
                case DOWN_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(DOWN), U(LGUI), END);
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(DOWN), U(LGUI), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(UP), U(LGUI), U(LSHIFT),   T(BSPACE),   END);
                            break;
                    }
                    break;

                case RIGHT_WHOLE_VECTOR:
                    switch (action)
                    {
                        case MOVE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(DOWN), U(LGUI), END);  // Same as non-whole
                            break;
                        case SELECT_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(UP), U(LGUI),   D(LSHIFT), D(LGUI), T(DOWN), U(LGUI), U(LSHIFT), END);
                            break;
                        case DELETE_ACTION:
                            pMacro = MACRO(I(10), D(LGUI), T(UP), U(LGUI),   D(LSHIFT), D(LGUI), T(DOWN), U(LGUI), U(LSHIFT),   T(BSPACE),   END);  // Completely equivalent to LEFT_WHOLE_VECTOR
                            break;
                    }
                    break;
            }
            break;
    }

    const bool noChars = (pMacro == NULL);
    if (noChars)
        return;

    action_macro_play(pMacro);
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

