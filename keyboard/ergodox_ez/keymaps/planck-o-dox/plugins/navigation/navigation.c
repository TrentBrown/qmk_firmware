// plugin
#include "../../plugin.h"

// tmk
#include "action_layer.h"

// navigation
#include "navigation.h"


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
    UP,
    DOWN,
    RIGHT,
    LEFT
 } NavigationDirection;


typedef enum NavigationUnit
{
    CHAR,
    PAGE,
    PARA,
    LINE,
    WORD,
    DOC,
 } NavigationUnit;


typedef enum NavigationSelection
{
    SELECT_NOTHING,
    SELECT_TO_BOUNDARY,
    SELECT_WHOLE,
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
                NavigationSetOrClearUnitAndSelection(PAGE, SELECT_WHOLE);
                break;
            case KC_W:
                NavigationSetOrClearUnitAndSelection(PARA, SELECT_WHOLE);
                break;
            case KC_E:
                NavigationSetOrClearUnitAndSelection(LINE, SELECT_WHOLE);
                break;
            case KC_R:
                NavigationSetOrClearUnitAndSelection(WORD, SELECT_WHOLE);
                break;
            case KC_T:
                NavigationSetOrClearUnitAndSelection(DOC, SELECT_WHOLE);
                break;

            // Select rest modifiers
            case KC_A:
                NavigationSetOrClearUnitAndSelection(PAGE, SELECT_TO_BOUNDARY);
                break;
            case KC_S:
                NavigationSetOrClearUnitAndSelection(PARA, SELECT_TO_BOUNDARY);
                break;
            case KC_D:
                NavigationSetOrClearUnitAndSelection(LINE, SELECT_TO_BOUNDARY);
                break;
            case KC_F:
                NavigationSetOrClearUnitAndSelection(WORD, SELECT_TO_BOUNDARY);
                break;
            case KC_G:
                NavigationSetOrClearUnitAndSelection(DOC, SELECT_TO_BOUNDARY);
                break;

            // Move modifiers
            case KC_Z:
                NavigationSetOrClearUnitAndSelection(PAGE, SELECT_NOTHING);
                break;
            case KC_X:
                NavigationSetOrClearUnitAndSelection(PARA, SELECT_NOTHING);
                break;
            case KC_C:
                NavigationSetOrClearUnitAndSelection(LINE, SELECT_NOTHING);
                break;
            case KC_V:
                NavigationSetOrClearUnitAndSelection(WORD, SELECT_NOTHING);
                break;
            case KC_B:
                NavigationSetOrClearUnitAndSelection(DOC, SELECT_NOTHING);
                break;

            // Navigation actions
            case KC_LEFT:
                if (navigation.event.pressed)
                    NavigationAction(LEFT);
                break;
            case KC_DOWN:
                if (navigation.event.pressed)
                    NavigationAction(DOWN);
                break;
            case KC_RIGHT:
                if (navigation.event.pressed)
                    NavigationAction(RIGHT);
                break;
            case KC_UP:
                if (navigation.event.pressed)
                    NavigationAction(UP);
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
    switch (navigation.machineselection)
    {
        case SELECT_NOTHING:
            break;

        case SELECT_TO_BOUNDARY:
            register_code(KC_LSHIFT);
            break;

        case SELECT_WHOLE:
            break;
    }

    switch (navigation.machine.unit)
    {
        case CHAR:
            break;
        case PAGE:
            break;
        case PARA:
            break;
        case LINE:
            break;
        case WORD:
            register_code(KC_LALT); // Option key on Mac. TODO: Windows?
            break;
        case DOC:
            break;
    }

    uint16_t code;
    switch (navigation.machinedirection)
    {
        case UP:
            code = KC_UP;
            break;

        case DOWN:
            code = KC_DOWN;
            break;

        case RIGHT:
            code = KC_RIGHT;
            break;

        case LEFT:
            code = KC_LEFT;
            break;
    }
    register_code(code);
    unregister_code(code);
}


void
NavigationSetOrClearUnitAndSelection
    {
        NavigationUnit unit,
        NavigationSelection selection
    }
{
    if (navigation.event.pressed)
    {
        navigation.machine.unit = unit;
        navigation.machine.selection = selection;
    }
    else if (navigation.event.released)
    {
        const bool someOtherNavKeyAlreadyPressed = (navigation.machine.state != state);
        if (someOtherNavKeyAlreadyPressed)
            return

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
    navigation.machine.unit = CHAR;
}

