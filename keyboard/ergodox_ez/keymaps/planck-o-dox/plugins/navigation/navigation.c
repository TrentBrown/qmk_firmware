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
typedef enum NavigationMachineState
{
    NORMAL_NAV_STATE,

    WHOLE_PAGE_NAV_STATE,
    WHOLE_PARA_NAV_STATE,
    WHOLE_LINE_NAV_STATE,
    WHOLE_WORD_NAV_STATE,
    WHOLE_DOC_NAV_STATE,

    SELECT_PAGE_NAV_STATE,
    SELECT_PARA_NAV_STATE,
    SELECT_LINE_NAV_STATE,
    SELECT_WORD_NAV_STATE,
    SELECT_DOC_NAV_STATE,

    MOVE_PAGE_NAV_STATE,
    MOVE_PARA_NAV_STATE,
    MOVE_LINE_NAV_STATE,
    MOVE_WORD_NAV_STATE,
    MOVE_DOC_NAV_STATE
} NavigationMachineState;


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
    PAGE_UNIT,
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
    NavigationMachineState state;
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
bool
NavigationBefore
    (
        keyrecord_t* pKeyRecord,
        action_t action
    );
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
    bool consumed = false;

    navigation.event.pKeyRecord = pKeyRecord;

    navigation.event.pressed = pKeyRecord->event.pressed;
    navigation.event.released = !navigation.event.pressed;
    navigation.event.code = action.code;

    uint8_t currentLayer = biton32(layer_state);

    if (currentLayer == navigation.settings.layer)
    {
        consumed = true;

        switch (navigation.event.code)
        {
            // Select whole modifiers
            case KC_Q:
                NavigationSetOrClearUnitAndSelection(PAGE_UNIT, WHOLE_UNIT_SELECTION);
                break;
            case KC_W:
                NavigationSetOrClearUnitAndSelection(PARA_UNIT, WHOLE_UNIT_SELECTION);
                break;
            case KC_E:
                NavigationSetOrClearUnitAndSelection(LINE_UNIT, WHOLE_UNIT_SELECTION);
                break;
            case KC_R:
                NavigationSetOrClearUnitAndSelection(WORD_UNIT, WHOLE_UNIT_SELECTION);
                break;
            case KC_T:
                NavigationSetOrClearUnitAndSelection(DOC_UNIT, WHOLE_UNIT_SELECTION);
                break;

            // Select rest modifiers
            case KC_A:
                NavigationSetOrClearUnitAndSelection(PAGE_UNIT, BOUNDARY_SELECTION);
                break;
            case KC_S:
                NavigationSetOrClearUnitAndSelection(PARA_UNIT, BOUNDARY_SELECTION);
                break;
            case KC_D:
                NavigationSetOrClearUnitAndSelection(LINE_UNIT, BOUNDARY_SELECTION);
                break;
            case KC_F:
                NavigationSetOrClearUnitAndSelection(WORD_UNIT, BOUNDARY_SELECTION);
                break;
            case KC_G:
                NavigationSetOrClearUnitAndSelection(DOC_UNIT, BOUNDARY_SELECTION);
                break;

            // Move modifiers
            case KC_Z:
                NavigationSetOrClearUnitAndSelection(PAGE_UNIT, NO_SELECTION);
                break;
            case KC_X:
                NavigationSetOrClearUnitAndSelection(PARA_UNIT, NO_SELECTION);
                break;
            case KC_C:
                NavigationSetOrClearUnitAndSelection(LINE_UNIT, NO_SELECTION);
                break;
            case KC_V:
                NavigationSetOrClearUnitAndSelection(WORD_UNIT, NO_SELECTION);
                break;
            case KC_B:
                NavigationSetOrClearUnitAndSelection(DOC_UNIT, NO_SELECTION);
                break;

            // Navigation actions
            case KC_LEFT:
                if (navigation.event.pressed)
                    NavigationAction(LEFT_DIRECTION);
                break;
            case KC_DOWN:
                if (navigation.event.pressed)
                    NavigationAction(DOWN_DIRECTION);
                break;
            case KC_RIGHT:
                if (navigation.event.pressed)
                    NavigationAction(RIGHT_DIRECTION);
                break;
            case KC_UP:
                if (navigation.event.pressed)
                    NavigationAction(UP_DIRECTION);
                break;

            default:
                consumed = false;
                break;
        }
    }

    return consumed;
}


void
NavigationAction(NavigationDirection direction)
{
    switch (navigation.machine.selection)
    {
        case NO_SELECTION:
            break;

        case BOUNDARY_SELECTION:
            register_code(KC_LSHIFT);
            break;

        case WHOLE_UNIT_SELECTION:
            break;
    }

    switch (navigation.machine.unit)
    {
        case CHAR_UNIT:
            break;
        case PAGE_UNIT:
            break;
        case PARA_UNIT:
            break;
        case LINE_UNIT:
            break;
        case WORD_UNIT:
            register_code(KC_LALT); // Option key on Mac. TODO: Windows?
            break;
        case DOC_UNIT:
            break;
    }

    uint16_t code;
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
    register_code(code);
    unregister_code(code);
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
        navigation.machine.unit = unit;
        navigation.machine.selection = selection;
    }
    else if (navigation.event.released)
    {
//        const bool someOtherNavKeyAlreadyPressed = (navigation.machine.state != state);
//        if (someOtherNavKeyAlreadyPressed)
//            return;

        // todo: do full clear here?
        navigation.machine.unit = NORMAL_NAV_STATE;
        navigation.machine.selection = NORMAL_NAV_STATE;
    }
}


bool
NavigationAfter
    (
        keyrecord_t* pKeyRecord,
        action_t action
    )
{
    bool consumed = false;

    return consumed;
}


void
NavigationClear(void)
{
    navigation.machine.state = NORMAL_NAV_STATE;
    navigation.machine.unit = CHAR_UNIT;
}

