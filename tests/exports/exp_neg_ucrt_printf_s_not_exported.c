/* TC
 * id:    exp_neg_ucrt_printf_s_not_exported
 * area:  exports
 * funcs:
 * procs: printf_s sprintf_s fprintf_s vsprintf_s vfprintf_s swprintf_s sscanf_s scanf_s fscanf_s _snprintf_s _vsnprintf_s _printf_l _sprintf_l
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs UCRT breaking changes (same page)
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
TC_CASE(secure_printf_absent)
{
    static const char *const v[] = { "printf_s", "sprintf_s", "fprintf_s", "vsprintf_s", "vfprintf_s", "swprintf_s" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(secure_scanf_absent)
{
    static const char *const v[] = { "sscanf_s", "scanf_s", "fscanf_s" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(locale_printf_absent)
{
    static const char *const v[] = { "_snprintf_s", "_vsnprintf_s", "_printf_l", "_sprintf_l" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control_present);
    TC_RUN(secure_printf_absent);
    TC_RUN(secure_scanf_absent);
    TC_RUN(locale_printf_absent);
    return tc_end();
}
