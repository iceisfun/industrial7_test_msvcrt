/* TC
 * id:    exp_neg_probe_pre120_startup_family_absent
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: malloc
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   UCRT startup design (appcrt/vcruntime split, Windows 10 SDK ucrt startup.h)
 * note:  PROBE: startup helpers introduced with the UCRT are believed absent in legacy CRTs.
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}
static void present(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (!p) tc_fail_expr(__FILE__, __LINE__, n);
}
static void check_list(const char *const *v, size_t n, int want)
{
    size_t i;
    for (i = 0; i < n; i++) { if (want) present(v[i]); else absent(v[i]); }
}
#define N(a) (sizeof(a) / sizeof((a)[0]))
static const char *const l_control[] = {
    "__set_app_type",
    "__getmainargs",
    "_cexit",
    "_exit",
};
static const char *const l_absent[] = {
    "_crt_atexit",
    "_crt_at_quick_exit",
    "_register_onexit_function",
    "_execute_onexit_table",
    "_register_thread_local_exe_atexit_callback",
    "_get_initial_narrow_environment",
    "_get_initial_wide_environment",
    "_configure_narrow_argv",
    "_initialize_narrow_environment",
    "_set_app_type",
};
TC_CASE(control) { CHECK(crt != NULL); check_list(l_control, N(l_control), 1); }
TC_CASE(absent) { CHECK(crt != NULL); check_list(l_absent, N(l_absent), 0); }

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control);
    TC_RUN(absent);
    return tc_end();
}
