/* TC
 * id:    exp_neg_ucrt_data_vars_probe
 * area:  exports
 * funcs:
 * procs: _environ _fmode _commode _daylight _timezone _tzname _pgmptr _wpgmptr _acmdln
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   inferred from CRT history: CRT global variables are not data exports in UCRT
 * note:  kind=negative. Probing: ucrtbase exposes __p__* accessors instead of the data symbols (see exp_ucrt_pseudo_vars_exported).
 */
#include "tc.h"

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}

TC_CASE(control_present) { CHECK(crt != NULL); CHECK(crt && GetProcAddress(crt, "malloc") != NULL); }
TC_CASE(environ_absent)
{
    static const char *const v[] = { "_environ" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(fmode_commode_absent)
{
    static const char *const v[] = { "_fmode", "_commode" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(time_globals_absent)
{
    static const char *const v[] = { "_daylight", "_timezone", "_tzname" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(pgmptr_acmdln_absent)
{
    static const char *const v[] = { "_pgmptr", "_wpgmptr", "_acmdln" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control_present);
    TC_RUN(environ_absent);
    TC_RUN(fmode_commode_absent);
    TC_RUN(time_globals_absent);
    TC_RUN(pgmptr_acmdln_absent);
    return tc_end();
}
