/* TC
 * id:    exp_neg_ucrt_removed_globals
 * area:  exports
 * funcs:
 * procs: _osver _winmajor _winminor _winver _osplatform
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs UCRT breaking changes / Global variables: _osver,_winmajor,_winminor,_winver,_osplatform removed; use GetVersionEx
 * note:  kind=negative.
 */
#include "tc.h"

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}

TC_CASE(control_present) { CHECK(crt != NULL); CHECK(crt && GetProcAddress(crt, "malloc") != NULL); }
TC_CASE(each_absent)
{
    static const char *const v[] = { "_osver", "_winmajor", "_winminor", "_winver", "_osplatform" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control_present);
    TC_RUN(each_absent);
    return tc_end();
}
