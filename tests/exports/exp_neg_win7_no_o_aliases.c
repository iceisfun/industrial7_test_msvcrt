/* TC
 * id:    exp_neg_win7_no_o_aliases
 * area:  exports
 * funcs:
 * procs: _o_malloc _o_free _o_fopen _o_exit
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  inferred
 * ref:   inferred from CRT history: _o_ aliases do not exist before UCRT
 * note:  kind=negative conf=inferred. The _o_ prefixed API-set forwarders are a UCRT (Windows 10) invention.
 */
#include "tc.h"

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}

TC_CASE(control_present) { CHECK(crt != NULL); CHECK(crt && GetProcAddress(crt, "malloc") != NULL); }
TC_CASE(malloc_free_absent)
{
    static const char *const v[] = { "_o_malloc", "_o_free" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(fopen_exit_absent)
{
    static const char *const v[] = { "_o_fopen", "_o_exit" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control_present);
    TC_RUN(malloc_free_absent);
    TC_RUN(fopen_exit_absent);
    return tc_end();
}
