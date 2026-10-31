/* TC
 * id:    exp_ucrt_startup_apis_exported
 * area:  exports
 * funcs: 
 * procs: _configure_narrow_argv _configure_wide_argv _initialize_narrow_environment _initialize_wide_environment _get_initial_narrow_environment _get_initial_wide_environment _initterm _initterm_e _crt_atexit _crt_at_quick_exit _register_onexit_function _execute_onexit_table _initialize_onexit_table _seh_filter_exe _seh_filter_dll __setusermatherr _set_app_type _controlfp_s _get_narrow_winmain_command_line _get_wide_winmain_command_line
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   UCRT startup API set (ucrt startup sources shipped with VS)
 */
#include "tc.h"

static void present(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = not exported */
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = unexpectedly exported */
}

static const char *const g_argv_env_group[] = { "_configure_narrow_argv", "_configure_wide_argv", "_initialize_narrow_environment", "_initialize_wide_environment", "_get_initial_narrow_environment", "_get_initial_wide_environment", NULL };
static const char *const g_onexit_group[] = { "_initterm", "_initterm_e", "_crt_atexit", "_crt_at_quick_exit", "_register_onexit_function", "_execute_onexit_table", "_initialize_onexit_table", NULL };
static const char *const g_exception_filter_group[] = { "_seh_filter_exe", "_seh_filter_dll", "__setusermatherr", NULL };
static const char *const g_app_type_group[] = { "_set_app_type", "_controlfp_s", NULL };
static const char *const g_winmain_cmdline_group[] = { "_get_narrow_winmain_command_line", "_get_wide_winmain_command_line", NULL };

TC_CASE(argv_env_group) { present(g_argv_env_group); }
TC_CASE(onexit_group) { present(g_onexit_group); }
TC_CASE(exception_filter_group) { present(g_exception_filter_group); }
TC_CASE(app_type_group) { present(g_app_type_group); }
TC_CASE(winmain_cmdline_group) { present(g_winmain_cmdline_group); }

int main(void)
{
    tc_begin();
    TC_RUN(argv_env_group);
    TC_RUN(onexit_group);
    TC_RUN(exception_filter_group);
    TC_RUN(app_type_group);
    TC_RUN(winmain_cmdline_group);
    return tc_end();
}
