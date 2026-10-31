/* TC
 * id:    exp_neg_ucrt_iob_not_exported_probe
 * area:  exports
 * funcs:
 * procs: _iob __iob_func __p__iob
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   inferred from CRT history: no _iob / __iob_func in ucrtbase
 * note:  kind=negative. Probing: UCRT replaced _iob with __acrt_iob_func; legacy symbols come only from legacy_stdio_definitions.lib.
 */
#include "tc.h"

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}

TC_CASE(control_present) { CHECK(crt != NULL); CHECK(crt && GetProcAddress(crt, "malloc") != NULL); }
TC_CASE(iob_data_absent)
{
    static const char *const v[] = { "_iob" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(iob_func_absent)
{
    static const char *const v[] = { "__iob_func", "__p__iob" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control_present);
    TC_RUN(iob_data_absent);
    TC_RUN(iob_func_absent);
    return tc_end();
}
