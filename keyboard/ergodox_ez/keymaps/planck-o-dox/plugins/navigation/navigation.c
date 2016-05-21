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


// State machine states
//typedef enum NavigationMachineState
//{
//    NORMAL_NAV_STATE,
//
//    WHOLE_PARA_NAV_STATE,
//    WHOLE_LINE_NAV_STATE,
//    WHOLE_WORD_NAV_STATE,
//    WHOLE_DOC_NAV_STATE,
//
//    SELECT_PARA_NAV_STATE,
//    SELECT_LINE_NAV_STATE,
//    SELECT_WORD_NAV_STATE,
//    SELECT_DOC_NAV_STATE,
//
//    MOVE_PARA_NAV_STATE,
//    MOVE_LINE_NAV_STATE,
//    MOVE_WORD_NAV_STATE,
//    MOVE_DOC_NAV_STATE
//} NavigationMachineState;


typedef enum NavigationDirection
{
    UP_DIRECTION,
    DOWN_DIRECTION,
    RIGHT_DIRECTION,
    LEFT_DIRECTION
 } NavigationDirection;


typedef enum NavigationUnit
{
    CHAR_UNIT,
    PARA_UNIT,
    LINE_UNIT,
    WORD_UNIT,
    DOC_UNIT,
 } NavigationUnit;


typedef enum NavigationSelection
{
    NO_SELECTION,
    BOUNDARY_SELECTION,
    WHOLE_UNIT_SELECTION,
 } NavigationSelection;


// State valid across multiple events
typedef struct NavigationMachine
{
    //NavigationMachineState state;
    uint16_t accleratorCode;
    NavigationUnit unit;
    NavigationSelection selection;
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

void NavigationClear(void);


bool
NavigationBefore
    (
        keyrecord_t* pKeyRecord,
        action_t action
    );
bool NavigationBeforeAction(void);
bool NavigationBeforeAccelerator(void);

bool
NavigationAfter
    (
        keyrecord_t* pKeyRecord,
        action_t action
    );

void
NavigationSetOrClearUnitAndSelection
    (
        NavigationUnit unit,
        NavigationSelection selection
    );

void
NavigationAction(NavigationDirection direction);


Plugin*
NavigationCreatePlugin(uint8_t layer)
{
    NavigationClear();

    navigation.settings.layer = layer;

    Plugin* pPlugin = (Plugin*)malloc(sizeof(Plugin));
    pPlugin->pName = "navigation";
    pPlugin->matrixScan = NULL;
    pPlugin->before = &NavigationBefore;
    pPlugin->after = &NavigationAfter;
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

    if (NavigationBeforeAction())
        return true;
    if (NavigationBeforeAccelerator())
        return true;

    return false;
}


bool
NavigationBeforeAction(void)
{
    switch (navigation.event.code)
    {
        case KC_LEFT:
            if (navigation.event.pressed)
                NavigationAction(LEFT_DIRECTION);
            return true;

        case KC_DOWN:
            if (navigation.event.pressed)
                NavigationAction(DOWN_DIRECTION);
            return true;

        case KC_RIGHT:
            if (navigation.event.pressed)
                NavigationAction(RIGHT_DIRECTION);
            return true;

        case KC_UP:
            if (navigation.event.pressed)
                NavigationAction(UP_DIRECTION);
            return true;
    }
    return false;
}

bool
NavigationBeforeAccelerator(void)
{
    switch (navigation.event.code)
    {
        // Select whole modifiers
        case KC_Q:
            NavigationSetOrClearUnitAndSelection(DOC_UNIT, WHOLE_UNIT_SELECTION);
            return true;
        case KC_W:
            NavigationSetOrClearUnitAndSelection(PARA_UNIT, WHOLE_UNIT_SELECTION);
            return true;
        case KC_E:
            NavigationSetOrClearUnitAndSelection(LINE_UNIT, WHOLE_UNIT_SELECTION);
            return true;
        case KC_R:
            NavigationSetOrClearUnitAndSelection(WORD_UNIT, WHOLE_UNIT_SELECTION);
            return true;

        // Select rest modifiers
        case KC_A:
            NavigationSetOrClearUnitAndSelection(DOC_UNIT, NO_SELECTION);
            return true;
        case KC_S:
            NavigationSetOrClearUnitAndSelection(PARA_UNIT, NO_SELECTION);
            return true;
        case KC_D:
            NavigationSetOrClearUnitAndSelection(LINE_UNIT, NO_SELECTION);
            return true;
        case KC_F:
            NavigationSetOrClearUnitAndSelection(WORD_UNIT, NO_SELECTION);
            return true;

        // Move modifiers
        case KC_Z:
            NavigationSetOrClearUnitAndSelection(DOC_UNIT, BOUNDARY_SELECTION);
            return true;
        case KC_X:
            NavigationSetOrClearUnitAndSelection(PARA_UNIT, BOUNDARY_SELECTION);
            return true;
        case KC_C:
            NavigationSetOrClearUnitAndSelection(LINE_UNIT, BOUNDARY_SELECTION);
            return true;
        case KC_V:
            NavigationSetOrClearUnitAndSelection(WORD_UNIT, BOUNDARY_SELECTION);
            return true;
    }
    return false;
}


void
NavigationAction(NavigationDirection direction)
{
    NavigationSelection selection = navigation.machine.selection;

    const macro_t* pMacro = NULL;

    switch (navigation.machine.unit)
    {
        case CHAR_UNIT:

            switch (direction)
            {
                case LEFT_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), T(LEFT), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LSHIFT), T(LEFT), U(LSHIFT), END);
                            break;
                        case WHOLE_UNIT_SELECTION:
                            break;
                    }
                    break;

                case RIGHT_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), T(RIGHT), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LSHIFT), T(RIGHT), U(LSHIFT), END);
                            break;
                        case WHOLE_UNIT_SELECTION:
                            break;
                    }
                    break;

                case UP_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), T(UP), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LSHIFT), T(UP), U(LSHIFT), END);
                            break;
                        case WHOLE_UNIT_SELECTION:
                            break;
                    }
                    break;

                case DOWN_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), T(DOWN), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LSHIFT), T(DOWN), U(LSHIFT), END);
                            break;
                        case WHOLE_UNIT_SELECTION:
                            break;
                    }
                    break;
            }
            break;

        case WORD_UNIT:
            switch (direction)
            {
                case LEFT_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), D(LALT), T(LEFT), U(LALT), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LALT), T(LEFT), U(LALT), U(LSHIFT), END);
                            break;
//                        case WHOLE_UNIT_SELECTION:
//                            pMacro = MACRO(I(10),   D(LALT), T(RIGHT), U(LALT),   D(LSHIFT), D(LALT), T(LEFT), U(LALT), U(LSHIFT),   END);
//                            break;
                        case WHOLE_UNIT_SELECTION:  //  one by one
                            pMacro = MACRO(I(10),   T(RIGHT),   D(LSHIFT), D(LALT), T(LEFT), U(LALT), U(LSHIFT),   END);
                            break;
                    }
                    break;

                case RIGHT_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), D(LALT), T(RIGHT), U(LALT), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LALT), T(RIGHT), U(LALT), U(LSHIFT), END);
                            break;
//                        case WHOLE_UNIT_SELECTION:
//                            pMacro = MACRO(I(10),   D(LALT), T(LEFT), U(LALT),   D(LSHIFT), D(LALT), T(RIGHT), U(LALT), U(LSHIFT),   END);
//                            break;
                        case WHOLE_UNIT_SELECTION:  //  one by one
                            pMacro = MACRO(I(10),   T(LEFT),   D(LSHIFT), D(LALT), T(RIGHT), U(LALT), U(LSHIFT),   END);
                            break;
                    }
                    break;

                case UP_DIRECTION:
                    break;

                case DOWN_DIRECTION:
                    break;
            }
            break;

        case LINE_UNIT:
            switch (direction)
            {
                case LEFT_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), D(LGUI), T(LEFT), U(LGUI), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(LEFT), U(LGUI), U(LSHIFT), END);
                            break;
                        case WHOLE_UNIT_SELECTION:
                            pMacro = MACRO(I(10),   T(RIGHT),   D(LSHIFT), D(LGUI), T(LEFT), U(LGUI), U(LSHIFT),   END);
                            break;
                    }
                    break;

                case RIGHT_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), D(LGUI), T(RIGHT), U(LGUI), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(RIGHT), U(LGUI), U(LSHIFT), END);
                            break;
                        case WHOLE_UNIT_SELECTION:
                            pMacro = MACRO(I(10),   T(LEFT),   D(LSHIFT), D(LGUI), T(RIGHT), U(LGUI), U(LSHIFT),   END);
                            break;
                    }
                    break;

                case UP_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10),   T(UP),   D(LGUI), T(LEFT), U(LGUI),   END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10),   D(LSHIFT), T(UP), U(LSHIFT),   D(LSHIFT), D(LGUI), T(LEFT), U(LGUI), U(LSHIFT),   END);
                            break;
                        case WHOLE_UNIT_SELECTION:
                            break;
                    }
                    break;

                case DOWN_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10),   T(DOWN),   D(LGUI), T(RIGHT), U(LGUI),   END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10),   D(LSHIFT), T(DOWN), U(LSHIFT),   D(LSHIFT), D(LGUI), T(RIGHT), U(LGUI), U(LSHIFT),   END);
                            break;
                        case WHOLE_UNIT_SELECTION:
                            break;
                    }
                    break;
            }
            break;

        case PARA_UNIT:
            switch (direction)
            {
                case LEFT_DIRECTION:
                case UP_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(A), U(LCTRL), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LALT), D(LSHIFT), T(UP), U(LSHIFT), U(LALT), END);
                            break;
                        case WHOLE_UNIT_SELECTION:
                            pMacro = MACRO(I(10),   D(LCTRL), T(E), U(LCTRL),   D(LALT), D(LSHIFT), T(UP), U(LSHIFT), U(LALT),   END);
                            break;
                    }
                    break;

                case RIGHT_DIRECTION:
                case DOWN_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), D(LCTRL), T(E), U(LCTRL), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LALT), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT), END);
                            break;
                        case WHOLE_UNIT_SELECTION:
                            pMacro = MACRO(I(10),   D(LCTRL), T(A), U(LCTRL),   D(LALT), D(LSHIFT), T(DOWN), U(LSHIFT), U(LALT),   END);
                            break;
                    }
                    break;
            }
            break;

        case DOC_UNIT:
            switch (direction)
            {
                case LEFT_DIRECTION:
                case UP_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), D(LGUI), T(UP), U(LGUI), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(UP), U(LGUI), U(LSHIFT), END);
                            break;
                        case WHOLE_UNIT_SELECTION:
                            break;
                    }
                    break;

                case RIGHT_DIRECTION:
                case DOWN_DIRECTION:
                    switch (selection)
                    {
                        case NO_SELECTION:
                            pMacro = MACRO(I(10), D(LGUI), T(DOWN), U(LGUI), END);
                            break;
                        case BOUNDARY_SELECTION:
                            pMacro = MACRO(I(10), D(LSHIFT), D(LGUI), T(DOWN), U(LGUI), U(LSHIFT), END);
                            break;
                        case WHOLE_UNIT_SELECTION:
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


void
NavigationSetOrClearUnitAndSelection
    (
        NavigationUnit unit,
        NavigationSelection selection
    )
{
    if (navigation.event.pressed)
    {
        navigation.machine.accleratorCode = navigation.event.code;
        navigation.machine.unit = unit;
        navigation.machine.selection = selection;
    }
    else if (navigation.event.released)
    {
        const bool someOtherAcceleratorKeyAlreadyPressed =
            (navigation.machine.accleratorCode != navigation.event.code);
        if (someOtherAcceleratorKeyAlreadyPressed)
            return;

        // todo: do full clear here?
        navigation.machine.accleratorCode = 0;
        navigation.machine.unit = CHAR_UNIT;
        navigation.machine.selection = NO_SELECTION;
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


void
NavigationClear(void)
{
    navigation.machine.accleratorCode = 0;
    navigation.machine.unit = CHAR_UNIT;
    navigation.machine.selection = NO_SELECTION;

    navigation.event.pKeyRecord = NULL;
    navigation.event.code = 0;
    navigation.event.pressed = false;
    navigation.event.released = false;
}

