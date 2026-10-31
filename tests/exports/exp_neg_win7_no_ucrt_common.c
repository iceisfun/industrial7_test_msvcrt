/* TC
 * id:    exp_neg_win7_no_ucrt_common
 * area:  exports
 * funcs:
 * procs: __stdio_common_vfprintf __stdio_common_vsprintf __stdio_common_vfscanf __stdio_common_vswprintf __conio_common_vcprintf __conio_common_vcscanf _configure_narrow_argv _initialize_onexit_table _initialize_narrow_environment
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  inferred
 * ref:   inferred from CRT history: UCRT-only internals absent from pre-UCRT CRTs
 * note:  kind=negative conf=inferred. __stdio_common_*, __conio_common_vcprintf, _configure_narrow_argv, and _initialize_onexit_table were introduced with the UCRT. Not claimed: __acrt_iob_func and _seh_filter_exe (also absent pre-UCRT, left out as uncertain).
 */
#include "tc.h"

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}

TC_CASE(control_present) { CHECK(crt != NULL); CHECK(crt && GetProcAddress(crt, "malloc") != NULL); }
TC_CASE(stdio_common_absent)
{
    static const char *const v[] = { "__stdio_common_vfprintf", "__stdio_common_vsprintf", "__stdio_common_vfscanf", "__stdio_common_vswprintf" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(conio_common_absent)
{
    static const char *const v[] = { "__conio_common_vcprintf", "__conio_common_vcscanf" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(startup_api_absent)
{
    static const char *const v[] = { "_configure_narrow_argv", "_initialize_onexit_table", "_initialize_narrow_environment" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control_present);
    TC_RUN(stdio_common_absent);
    TC_RUN(conio_common_absent);
    TC_RUN(startup_api_absent);
    return tc_end();
}
