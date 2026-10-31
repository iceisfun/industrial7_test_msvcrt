/* TC
 * id:    exp_neg_probe_quick_exit_pre140
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: malloc
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs quick_exit: new in the Universal CRT (VS2015)
 * note:  PROBE: quick_exit/at_quick_exit are believed absent from every pre-UCRT CRT including msvcr120.
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
    "malloc",
    "exit",
    "atexit",
};
static const char *const l_absent[] = {
    "quick_exit",
    "at_quick_exit",
    "_crt_at_quick_exit",
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
