// system
#include <stdlib.h>

// plugin
#include "../../plugin.h"

// navigation
#include "reset.h"


// Forward declarations of local functions
bool ResetBefore
    (
        keyrecord_t* pKeyRecord,
        action_t action
    );


Plugin*
ResetCreatePlugin(void)
{
    Plugin* pPlugin = (Plugin*)malloc(sizeof(Plugin));
    pPlugin->pName = "reset";
    pPlugin->matrixScan = NULL;
    pPlugin->before = &ResetBefore;
    pPlugin->after = NULL;
    pPlugin->reset = NULL;
    pPlugin->pPrevPlugin = NULL;
    pPlugin->pNextPlugin = NULL;
    return pPlugin;
}

bool
ResetBefore
    (
        keyrecord_t* pKeyRecord,
        action_t action
    )
{
    bool consumed = false;

    if (action.code == KC_ESCAPE)
    {
        if (pKeyRecord->event.pressed)
            PluginResetAll();
    }

    return consumed;
}
