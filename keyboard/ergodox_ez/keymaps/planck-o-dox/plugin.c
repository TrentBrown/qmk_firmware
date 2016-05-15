// plugin
#include "plugin.h"

Plugin* p_head_plugin = NULL;

void
push_plugin(Plugin* p_plugin)
{
    const bool listIsEmpty = (p_head_plugin == NULL);
    if (listIsEmpty)
    {
        p_plugin->p_prev_plugin = NULL;
        p_plugin->p_next_plugin = NULL;
    }
    else
    {
        p_plugin->p_prev_plugin = NULL;
        p_plugin->p_next_plugin = p_head_plugin;

        p_head_plugin->p_prev_plugin = p_plugin;
    }

    p_head_plugin = p_plugin;
}


void
add_plugin_before(Plugin* p_plugin, Plugin* p_other_plugin)
{
    p_plugin->p_prev_plugin = p_other_plugin->p_prev_plugin;
    p_plugin->p_next_plugin = p_other_plugin;

    p_other_plugin->p_prev_plugin = p_plugin;

    if (p_head_plugin == p_other_plugin)
        p_head_plugin = p_plugin;
}


void
add_plugin_after(Plugin* p_plugin, Plugin* p_other_plugin)
{
    p_plugin->p_prev_plugin = p_other_plugin;
    p_plugin->p_next_plugin = p_other_plugin->p_next_plugin;

    p_other_plugin->p_next_plugin = p_plugin;
}


bool
has_plugin(const char* p_name)
{
    return (find_plugin_named(p_name) != NULL);
}


Plugin*
find_plugin_named(const char* p_name)
{
    Plugin* p_plugin;
    for (p_plugin = p_head_plugin;
         p_plugin != NULL;
         p_plugin = p_plugin->p_next_plugin)
    {
        if (strcmp(p_plugin->p_name, p_name) == 0)
            return p_plugin;
    }
    return NULL;
}


void
plugin_matrix_scan(void)
{
    Plugin* p_plugin;
    for (p_plugin = p_head_plugin;
         p_plugin != NULL;
         p_plugin = p_plugin->p_next_plugin)
    {
        plugin_matrix_scan_function plugin_matrix_scan = p_plugin->matrix_scan;
        if (plugin_matrix_scan == NULL)
            continue;

        plugin_matrix_scan();
    }
}


bool
plugin_process_action_before_hook
    (
        keyrecord_t* p_keyrecord,
        action_t action
    )
{
    Plugin* p_plugin;
    for (p_plugin = p_head_plugin;
         p_plugin != NULL;
         p_plugin = p_plugin->p_next_plugin)
    {
        plugin_before_function plugin_before = p_plugin->before;
        if (plugin_before == NULL)
            continue;

        const bool consumed = plugin_before(p_keyrecord, action);
        if (consumed)
            return true;
    }

    return false;
}


bool
plugin_process_action_after_hook
    (
        keyrecord_t* p_keyrecord,
        action_t action
    )
{
    Plugin* p_plugin;
    for (p_plugin = p_head_plugin;
         p_plugin != NULL;
         p_plugin = p_plugin->p_next_plugin)
    {
        plugin_after_function plugin_after = p_plugin->after;
        if (plugin_after == NULL)
            continue;

        const bool consumed = plugin_after(p_keyrecord, action);
        if (consumed)
            return true;
    }

    return false;
}
