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

    WHOLE_WORD_NAV_STATE,
    WHOLE_LINE_NAV_STATE,
    WHOLE_PARA_NAV_STATE,
    WHOLE_PAGE_NAV_STATE,
    WHOLE_DOC_NAV_STATE,

    SELECT_WORD_NAV_STATE,
    SELECT_LINE_NAV_STATE,
    SELECT_PARA_NAV_STATE,
    SELECT_PAGE_NAV_STATE,
    SELECT_DOC_NAV_STATE,

    MOVE_WORD_NAV_STATE,
    MOVE_LINE_NAV_STATE,
    MOVE_PARA_NAV_STATE,
    MOVE_PAGE_NAV_STATE,
    MOVE_DOC_NAV_STATE
 } NavigationMachineState;


// State valid across multiple events
typedef struct NavigationMachine
{
    NavigationMachineState state;
} NavigationMachine;


// The global Navigation state
typedef struct Navigation
{
    NavigationSettings settings;
    NavigationMachine machine;
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

    uint8_t currentLayer = biton32(layer_state);

    if (currentLayer == navigation.settings.layer)
    {
        consumed = true;

        switch (action.code)
        {
            // Select whole
            case KC_Q:
                break;
            case KC_W:
                break;
            case KC_E:
                break;
            case KC_R:
                break;
            case KC_T:
                break;

            // Select rest
            case KC_A:
                break;
            case KC_S:
                break;
            case KC_D:
                break;
            case KC_F:
                break;
            case KC_G:
                break;

            // Move
            case KC_Z:
                break;
            case KC_X:
                break;
            case KC_C:
                break;
            case KC_V:
                break;
            case KC_B:
                break;

            default:
                consumed = false;
                break;
        }
    }

    return consumed;
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