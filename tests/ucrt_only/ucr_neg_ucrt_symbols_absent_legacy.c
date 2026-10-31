/* TC
 * id:    ucr_neg_ucrt_symbols_absent_legacy
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf timespec_get _configure_narrow_argv __acrt_iob_func _o_malloc quick_exit at_quick_exit _timespec64_get
 * crt:   win7
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs: UCRT introduced in VS2015 / Windows 10; these are UCRT-only exports
 * note:  Names chosen because they are certainly not exported by msvcrt/msvcr80..msvcr120. Names with doubtful history (_set_app_type, _seh_filter_exe, _crt_atexit) are deliberately left out.
 */
#include "tc.h"

TC_CASE(vsprintf_common) { CHECK(tc_crt_proc("__stdio_common_vsprintf") == NULL); }
TC_CASE(acrt_iob_func) { CHECK(tc_crt_proc("__acrt_iob_func") == NULL); }
TC_CASE(timespec) { CHECK(tc_crt_proc("timespec_get") == NULL); CHECK(tc_crt_proc("_timespec64_get") == NULL); }
TC_CASE(configure_argv) { CHECK(tc_crt_proc("_configure_narrow_argv") == NULL); }
TC_CASE(o_malloc) { CHECK(tc_crt_proc("_o_malloc") == NULL); }
TC_CASE(quick_exit) { CHECK(tc_crt_proc("quick_exit") == NULL); CHECK(tc_crt_proc("at_quick_exit") == NULL); }
int main(void)
{
    tc_begin();
    TC_RUN(vsprintf_common); TC_RUN(acrt_iob_func); TC_RUN(timespec);
    TC_RUN(configure_argv); TC_RUN(o_malloc); TC_RUN(quick_exit);
    return tc_end();
}
