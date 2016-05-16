// plugin
#include "plugin.h"

Plugin* gpHeadPlugin = NULL;

void
PluginAdd(Plugin* pPlugin)
{
    const bool listIsEmpty = (gpHeadPlugin == NULL);
    if (listIsEmpty)
    {
        pPlugin->pPrevPlugin = NULL;
        pPlugin->pNextPlugin = NULL;
    }
    else
    {
        pPlugin->pPrevPlugin = NULL;
        pPlugin->pNextPlugin = gpHeadPlugin;

        gpHeadPlugin->pPrevPlugin = pPlugin;
    }

    gpHeadPlugin = pPlugin;
}


void
PluginAddBefore(Plugin* pPlugin, Plugin* pOtherPlugin)
{
    pPlugin->pPrevPlugin = pOtherPlugin->pPrevPlugin;
    pPlugin->pNextPlugin = pOtherPlugin;

    pOtherPlugin->pPrevPlugin = pPlugin;

    if (gpHeadPlugin == pOtherPlugin)
        gpHeadPlugin = pPlugin;
}


void
PluginAddAfter(Plugin* pPlugin, Plugin* pOtherPlugin)
{
    pPlugin->pPrevPlugin = pOtherPlugin;
    pPlugin->pNextPlugin = pOtherPlugin->pNextPlugin;

    pOtherPlugin->pNextPlugin = pPlugin;
}


Plugin*
PluginFindNamed(const char* pName)
{
    Plugin* pPlugin;
    for (pPlugin = gpHeadPlugin;
         pPlugin != NULL;
         pPlugin = pPlugin->pNextPlugin)
    {
        if (strcmp(pPlugin->pName, pName) == 0)
            return pPlugin;
    }
    return NULL;
}


void
plugin_matrix_scan(void)
{
    Plugin* pPlugin;
    for (pPlugin = gpHeadPlugin;
         pPlugin != NULL;
         pPlugin = pPlugin->pNextPlugin)
    {
        PluginMatrixScanFunction plugin_matrix_scan = pPlugin->matrixScan;
        if (plugin_matrix_scan == NULL)
            continue;

        plugin_matrix_scan();
    }
}


bool
plugin_process_action_before_hook
    (
        keyrecord_t* pKeyRecord,
        action_t action
    )
{
    Plugin* pPlugin;
    for (pPlugin = gpHeadPlugin;
         pPlugin != NULL;
         pPlugin = pPlugin->pNextPlugin)
    {
        PluginBeforeFunction pluginBefore = pPlugin->before;
        if (pluginBefore == NULL)
            continue;

        const bool consumed = pluginBefore(pKeyRecord, action);
        if (consumed)
            return true;
    }

    return false;
}


bool
plugin_process_action_after_hook
    (
        keyrecord_t* pKeyRecord,
        action_t action
    )
{
    Plugin* pPlugin;
    for (pPlugin = gpHeadPlugin;
         pPlugin != NULL;
         pPlugin = pPlugin->pNextPlugin)
    {
        PluginAfterFunction pluginAfter = pPlugin->after;
        if (pluginAfter == NULL)
            continue;

        const bool consumed = pluginAfter(pKeyRecord, action);
        if (consumed)
            return true;
    }

    return false;
}
