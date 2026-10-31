/* TC
 * id:    ucr_stdio_vsprintf_exponent_digits
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (exponent has at least two digits); UCRT default is not the legacy three-digit form (option 0x20)
 */
#include "tc.h"
#include <stdarg.h>

typedef int (__cdecl *vs_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vs_t p_vs;
static volatile double d5 = 1e5, dm5 = 1e-5, d100 = 1e100, d6 = 1e6, dm100 = 1e-100;
static int calls(char *b, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vs(0, b, 64, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(e_forms)
{
    char b[64]; int r;
    if (!p_vs) TC_SKIP("not exported");
    r = calls(b, "%e", d5);   CHECK_STR(b, "1.000000e+05"); CHECK_EQ_I(r, 12);
    r = calls(b, "%e", dm5);  CHECK_STR(b, "1.000000e-05"); CHECK_EQ_I(r, 12);
    r = calls(b, "%e", d100); CHECK_STR(b, "1.000000e+100"); CHECK_EQ_I(r, 13);
    calls(b, "%e", dm100);    CHECK_STR(b, "1.000000e-100");
}
TC_CASE(E_and_g)
{
    char b[64];
    if (!p_vs) TC_SKIP("not exported");
    calls(b, "%E", d5);  CHECK_STR(b, "1.000000E+05");
    calls(b, "%g", dm5); CHECK_STR(b, "1e-05");
    calls(b, "%g", d100); CHECK_STR(b, "1e+100");
    calls(b, "%g", d5);  CHECK_STR(b, "100000");
    calls(b, "%g", d6);  CHECK_STR(b, "1e+06");
    calls(b, "%G", dm5); CHECK_STR(b, "1E-05");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vs = (vs_t)tc_crt_proc("__stdio_common_vsprintf");
    TC_RUN(e_forms); TC_RUN(E_and_g);
    return tc_end();
}
