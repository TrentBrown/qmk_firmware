// plugin
#include "../../plugin.h"

// tmk
#include "action_layer.h"

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
//        case KC_W:
//            NavigationSetOrClearUnitAndSelection(PARA_UNIT, WHOLE_UNIT_SELECTION);
//            return true;
//        case KC_E:
//            NavigationSetOrClearUnitAndSelection(LINE_UNIT, WHOLE_UNIT_SELECTION);
//            return true;
//        case KC_R:
//            NavigationSetOrClearUnitAndSelection(WORD_UNIT, WHOLE_UNIT_SELECTION);
//            return true;
//        case KC_T:
//            NavigationSetOrClearUnitAndSelection(DOC_UNIT, WHOLE_UNIT_SELECTION);
//            return true;

        // Select rest modifiers
        case KC_S:
            NavigationSetOrClearUnitAndSelection(PARA_UNIT, NO_SELECTION);
            return true;
        case KC_D:
            NavigationSetOrClearUnitAndSelection(LINE_UNIT, NO_SELECTION);
            return true;
        case KC_F:
            NavigationSetOrClearUnitAndSelection(WORD_UNIT, NO_SELECTION);
            return true;
        case KC_G:
            NavigationSetOrClearUnitAndSelection(DOC_UNIT, NO_SELECTION);
            return true;

        // Move modifiers
        case KC_X:
            NavigationSetOrClearUnitAndSelection(PARA_UNIT, BOUNDARY_SELECTION);
            return true;
        case KC_C:
            NavigationSetOrClearUnitAndSelection(LINE_UNIT, BOUNDARY_SELECTION);
            return true;
        case KC_V:
            NavigationSetOrClearUnitAndSelection(WORD_UNIT, BOUNDARY_SELECTION);
            return true;
        case KC_B:
            NavigationSetOrClearUnitAndSelection(DOC_UNIT, BOUNDARY_SELECTION);
            return true;
    }
    return false;
}

void
NavigationAction(NavigationDirection direction)
{
    bool shift = false;
    bool option = false;
    bool command = false;
    bool control = false;
    
    NavigationSelection selection = navigation.machine.selection;

    uint16_t code = 0;
    switch (navigation.machine.unit)
    {
        case CHAR_UNIT:
            if (selection == BOUNDARY_SELECTION)
                shift = true;
            switch (direction)
            {
                case UP_DIRECTION:
                    code = KC_UP;
                    break;

                case DOWN_DIRECTION:
                    code = KC_DOWN;
                    break;

                case RIGHT_DIRECTION:
                    code = KC_RIGHT;
                    break;

                case LEFT_DIRECTION:
                    code = KC_LEFT;
                    break;
            }
            break;

        case PARA_UNIT:
            if (selection == BOUNDARY_SELECTION)
            {
                shift = true;
                option = true;
                switch (direction)
                {
                    case UP_DIRECTION:
                    case LEFT_DIRECTION:
                        code = KC_UP;
                        break;

                    case DOWN_DIRECTION:
                    case RIGHT_DIRECTION:
                        code = KC_DOWN;
                        break;
                }
            }
            else // No selection
            {
                control = true;
                switch (direction)
                {
                    case UP_DIRECTION:
                    case LEFT_DIRECTION:
                        code = KC_A;
                        break;

                    case DOWN_DIRECTION:
                    case RIGHT_DIRECTION:
                        code = KC_E;
                        break;
                }
            }
            break;

        case LINE_UNIT:
            switch (direction)
            {
                case UP_DIRECTION:
                    if (selection == BOUNDARY_SELECTION)
                        shift = true;
                    code = KC_UP;
                    break;

                case DOWN_DIRECTION:
                    if (selection == BOUNDARY_SELECTION)
                        shift = true;
                    code = KC_DOWN;
                    break;

                case RIGHT_DIRECTION:
                    command = true;
                    if (selection == BOUNDARY_SELECTION)
                        shift = true;
                    code = KC_RIGHT;
                    break;

                case LEFT_DIRECTION:
                    command = true;
                    if (selection == BOUNDARY_SELECTION)
                        shift = true;
                    code = KC_LEFT;
                    break;
            }
            break;

        case WORD_UNIT:
            switch (direction)
            {
                case UP_DIRECTION:
                    break;

                case DOWN_DIRECTION:
                    break;

                case RIGHT_DIRECTION:
                    option = true;
                    if (selection == BOUNDARY_SELECTION)
                        shift = true;
                    code = KC_RIGHT;
                    break;

                case LEFT_DIRECTION:
                    option = true;
                    if (selection == BOUNDARY_SELECTION)
                        shift = true;
                    code = KC_LEFT;
                    break;
            }
            break;

        case DOC_UNIT:
            command = true;
            if (selection == BOUNDARY_SELECTION)
                shift = true;
            switch (direction)
            {
                case UP_DIRECTION:
                case LEFT_DIRECTION:
                    code = KC_UP;
                    break;

                case DOWN_DIRECTION:
                case RIGHT_DIRECTION:
                    code = KC_DOWN;
                    break;
            }
            break;
    }

    if (code == 0)
        return;

    if (shift)
        register_code(KC_LSHIFT);
    if (option)
        register_code(KC_LALT);
    if (command)
        register_code(KC_LGUI);
    if (control)
        register_code(KC_LCTL);

    register_code(code);
    unregister_code(code);

    if (control)
        unregister_code(KC_LCTL);
    if (command)
        unregister_code(KC_LGUI);
    if (option)
        unregister_code(KC_LALT);
    if (shift)
        unregister_code(KC_LSHIFT);
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

