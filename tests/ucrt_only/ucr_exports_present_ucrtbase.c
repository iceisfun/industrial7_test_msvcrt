/* TC
 * id:    ucr_exports_present_ucrtbase
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf __stdio_common_vsscanf __acrt_iob_func _crt_atexit _initialize_onexit_table _register_onexit_function _execute_onexit_table _configure_narrow_argv _configure_wide_argv _initialize_narrow_environment _get_initial_narrow_environment _set_app_type _get_narrow_winmain_command_line _seh_filter_exe _set_new_mode _timespec64_get quick_exit _crt_at_quick_exit _Exit _aligned_recalloc
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs for each function (UCRT); startup entry points per the VS ucrt startup sources / ucrtbase export table
 * note:  at_quick_exit is deliberately NOT listed: in ucrt it is a static wrapper over the exported _crt_at_quick_exit.
 *        Positive counterpart to ucr_neg_ucrt_symbols_absent_legacy.
 * Class D: timespec_get is NOT in the Win10 22H2 ucrtbase export table (raw: "missing export: timespec_get"); like at_quick_exit it is a static inline in <time.h>
 *        over the exported _timespec64_get, so it was removed from the name set.
 */
#include "tc.h"

static const char *const names[] = {
    "__stdio_common_vsprintf", "__stdio_common_vsscanf", "__acrt_iob_func", "_crt_atexit",
    "_initialize_onexit_table", "_register_onexit_function", "_execute_onexit_table",
    "_configure_narrow_argv", "_configure_wide_argv", "_initialize_narrow_environment",
    "_get_initial_narrow_environment", "_set_app_type", "_get_narrow_winmain_command_line",
    "_seh_filter_exe", "_set_new_mode", "_timespec64_get", "quick_exit",
    "_crt_at_quick_exit", "_Exit", "_aligned_recalloc"
};

TC_CASE(all_exported)
{
    unsigned i;
    for (i = 0; i < sizeof names / sizeof names[0]; i++)
        if (!tc_crt_proc(names[i])) { tc_failhdr(__FILE__, __LINE__); tc_write("missing export: "); tc_write(names[i]); tc_write("\n"); }
}
int main(void) { tc_begin(); TC_RUN(all_exported); return tc_end(); }
