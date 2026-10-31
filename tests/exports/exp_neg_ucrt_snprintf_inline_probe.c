/* TC
 * id:    exp_neg_ucrt_snprintf_inline_probe
 * area:  exports
 * funcs:
 * procs: _snprintf snprintf _vsnprintf vsnprintf _snwprintf _scprintf _vscprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   inferred from CRT history: snprintf-family exports in ucrtbase
 * note:  kind=negative. Probing: _snprintf/_vsnprintf/snprintf/_scprintf/_vscprintf/_snwprintf are inline over __stdio_common_vsprintf in UCRT headers, so ucrtbase.dll does not export them (mingw import lib lists some but may supply them itself).
 */
#include "tc.h"

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}

TC_CASE(control_present) { CHECK(crt != NULL); CHECK(crt && GetProcAddress(crt, "malloc") != NULL); }
TC_CASE(snprintf_absent)
{
    static const char *const v[] = { "_snprintf", "snprintf", "_vsnprintf", "vsnprintf", "_snwprintf" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(vscprintf_absent)
{
    static const char *const v[] = { "_scprintf", "_vscprintf" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control_present);
    TC_RUN(snprintf_absent);
    TC_RUN(vscprintf_absent);
    return tc_end();
}
