#ifndef __PLUGIN
#define __PLUGIN

typedef void (*PluginMatrixScanFunction)(void);
typedef bool (*PluginBeforeFunction)(keyrecord_t* pKeyRecord, action_t action);
typedef bool (*PluginAfterFunction)(keyrecord_t* pKeyRecord, action_t action);

typedef struct Plugin
{
    char* pName;
    PluginMatrixScanFunction matrixScan;
    PluginBeforeFunction before;
    PluginAfterFunction after;
    struct Plugin* pPrevPlugin;
    struct Plugin* pNextPlugin;
} Plugin;


void AddPlugin(Plugin* pPlugin);
void AddPluginBefore(Plugin* pPlugin, Plugin* pOtherPlugin);
void AddPluginAfter(Plugin* pPlugin, Plugin* pOtherPlugin);

Plugin* FindPluginNamed(const char* pName);


void plugin_matrix_scan(void);  // Call from your matrix_scan_user() function.

bool
plugin_process_action_before_hook
    (
        keyrecord_t* pKeyRecord,
        action_t action
    );

bool
plugin_process_action_after_hook
    (
        keyrecord_t* pKeyRecord,
        action_t action
    );

#endif //__PLUGIN
