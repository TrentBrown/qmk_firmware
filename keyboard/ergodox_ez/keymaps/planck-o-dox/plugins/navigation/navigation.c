// plugin
#include "../../plugin.h"

// tmk
#include "action_layer.h"

// navigation
#include "navigation.h"


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


// State valid across multiple events
typedef struct NavigationMachine
{
    NavigationMachineState state;
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
            // Select whole
            case KC_Q:
                NavigationSetOrClear(WHOLE_PAGE_NAV_STATE);
                break;
            case KC_W:
                NavigationSetOrClear(WHOLE_PARA_NAV_STATE);
                break;
            case KC_E:
                NavigationSetOrClear(WHOLE_LINE_NAV_STATE);
                break;
            case KC_R:
                NavigationSetOrClear(WHOLE_WORD_NAV_STATE);
                break;
            case KC_T:
                NavigationSetOrClear(WHOLE_DOC_NAV_STATE);
                break;

            // Select rest
            case KC_A:
                NavigationSetOrClear(SELECT_PAGE_NAV_STATE);
                break;
            case KC_S:
                NavigationSetOrClear(SELECT_PARA_NAV_STATE);
                break;
            case KC_D:
                NavigationSetOrClear(SELECT_LINE_NAV_STATE);
                break;
            case KC_F:
                NavigationSetOrClear(SELECT_WORD_NAV_STATE);
                break;
            case KC_G:
                NavigationSetOrClear(SELECT_DOC_NAV_STATE);
                break;

            // Move
            case KC_Z:
                NavigationSetOrClear(MOVE_PAGE_NAV_STATE);
                break;
            case KC_X:
                NavigationSetOrClear(MOVE_PARA_NAV_STATE);
                break;
            case KC_C:
                NavigationSetOrClear(MOVE_LINE_NAV_STATE);
                break;
            case KC_V:
                NavigationSetOrClear(MOVE_WORD_NAV_STATE);
                break;
            case KC_B:
                NavigationSetOrClear(MOVE_DOC_NAV_STATE);
                break;

            // Navigation
            case KC_LEFT:
                if (navigation.event.pressed)
                {
                    register_code(KC_LEFT);
                    unregister_code(KC_LEFT);
                }
                break;
            case KC_DOWN:
                if (navigation.event.pressed)
                {
                    register_code(KC_DOWN);
                    unregister_code(KC_DOWN);
                }
                break;
            case KC_RIGHT:
                if (navigation.event.pressed)
                {
                    register_code(KC_RIGHT);
                    unregister_code(KC_RIGHT);
                }
                break;
            case KC_UP:
                if (navigation.event.pressed)
                {
                    register_code(KC_UP);
                    unregister_code(KC_UP);
                }
                break;

            default:
                consumed = false;
                break;
        }
    }

    return consumed;
}


void
NavigationSetOrClear(NavigationMachineState state)
{
    if (navigation.event.pressed)
    {
        navigation.machine.state = state;
    }
    else // was released
    {
        const bool someOtherNavKeyAlreadyPressed = (navigation.machine.state != state);
        if (someOtherNavKeyAlreadyPressed)
            return

        navigation.machine.state = NORMAL_NAV_STATE;
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