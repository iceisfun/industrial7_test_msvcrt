/* TC
 * id:    exp_neg_ucrt_conio_printf_not_exported
 * area:  exports
 * funcs:
 * procs: _cprintf _cscanf _cwprintf _cscanf_s _cprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  documented
 * ref:   MS docs UCRT breaking changes (conio printf/scanf are inline over __conio_common_*)
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
    static const char *const v[] = { "_cprintf", "_cscanf", "_cwprintf", "_cscanf_s", "_cprintf_s" };
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
