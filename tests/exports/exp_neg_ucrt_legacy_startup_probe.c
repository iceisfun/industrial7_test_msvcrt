/* TC
 * id:    exp_neg_ucrt_legacy_startup_probe
 * area:  exports
 * funcs:
 * procs: __set_app_type __getmainargs __wgetmainargs _amsg_exit _XcptFilter
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   inferred from CRT history: msvcrt-era startup helpers absent from ucrtbase
 * note:  kind=negative. Probing: UCRT uses _set_app_type/_configure_*_argv; mingw-w64 supplies __getmainargs itself for ucrt, so the import lib is not proof.
 */
#include "tc.h"

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}

TC_CASE(control_present) { CHECK(crt != NULL); CHECK(crt && GetProcAddress(crt, "malloc") != NULL); }
TC_CASE(set_app_type_absent)
{
    static const char *const v[] = { "__set_app_type" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(getmainargs_absent)
{
    static const char *const v[] = { "__getmainargs", "__wgetmainargs", "_amsg_exit" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(xcptfilter_absent)
{
    static const char *const v[] = { "_XcptFilter" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control_present);
    TC_RUN(set_app_type_absent);
    TC_RUN(getmainargs_absent);
    TC_RUN(xcptfilter_absent);
    return tc_end();
}
