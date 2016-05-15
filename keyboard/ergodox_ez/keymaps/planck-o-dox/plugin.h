#ifndef __PLUGIN
#define __PLUGIN

typedef void (*plugin_matrix_scan_function)(void);
typedef bool (*plugin_before_function)(keyrecord_t* p_keyrecord, action_t action);
typedef bool (*plugin_after_function)(keyrecord_t* p_keyrecord, action_t action);

typedef struct Plugin
{
    char* p_name;
    plugin_matrix_scan_function matrix_scan;
    plugin_before_function before;
    plugin_after_function after;
    struct Plugin* p_prev_plugin;
    struct Plugin* p_next_plugin;
} Plugin;


void push_plugin(Plugin* p_plugin);
void add_plugin_before(Plugin* p_plugin, Plugin* p_other_plugin);
void add_plugin_after(Plugin* p_plugin, Plugin* p_other_plugin);

bool has_plugin(const char* p_name);

Plugin* find_plugin_named(const char* p_name);


void plugin_matrix_scan(void);  // Call from your matrix_scan_user() function.

bool
plugin_process_action_before_hook
    (
        keyrecord_t* p_keyrecord,
        action_t action
    );

bool
plugin_process_action_after_hook
    (
        keyrecord_t* p_keyrecord,
        action_t action
    );

#endif //__PLUGIN
