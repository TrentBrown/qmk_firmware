// plugin
#include "plugin.h"

Plugin* p_first_plugin = NULL;

void
add_plugin(Plugin* p_plugin)
{
    p_plugin->p_next_plugin = p_first_plugin;
    p_first_plugin = p_plugin;
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
    for (p_plugin = p_first_plugin;
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
//    Plugin* p_plugin;
//    for (p_plugin = p_first_plugin;
//         p_plugin != NULL;
//         p_plugin = p_plugin->p_next_plugin)
//    {
//        plugin_matrix_scan_function matrix_scan = p_plugin->matrix_scan;
//        if (matrix_scan == NULL)
//            continue;
//
//        matrix_scan();
//    }
}


bool
plugin_process_action_before_hook
    (
        keyrecord_t* p_keyrecord,
        action_t action
    )
{
    Plugin* p_plugin;
    for (p_plugin = p_first_plugin;
         p_plugin != NULL;
         p_plugin = p_plugin->p_next_plugin)
    {
        plugin_before_function before = p_plugin->before;
        if (before == NULL)
            continue;

        const bool consumed = before(p_keyrecord, action);
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
    if (p_keyrecord->event.pressed)
        ergodox_right_led_2_on();  // Green
    else
        ergodox_led_all_off();

    Plugin* p_plugin;
    for (p_plugin = p_first_plugin;
         p_plugin != NULL;
         p_plugin = p_plugin->p_next_plugin)
    {
        plugin_after_function after = p_plugin->after;
        if (after == NULL)
            continue;

        const bool consumed = after(p_keyrecord, action);
        if (consumed)
            return true;
    }

    return false;
}
