/* TC
 * id:    pfo_vsprintf_opt_3digit_exp
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  inferred
 * ref:   corecrt_stdio_config.h _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS; MS docs _set_output_format (legacy CRTs print 3-digit exponents)
 * note:  options=0x10 selects legacy three-digit exponents.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int vw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

static char a[64], c[64];
static volatile double d15 = 1.5, d1e100 = 1e100, d1e5 = 1e-5, d1 = 1.0;

TC_CASE(two_digit_default)  { vs(0, a, sizeof a, "%e", d15); CHECK_STR(a, "1.500000e+00"); }
TC_CASE(three_digit_opt)    { vs(0x10, a, sizeof a, "%e", d15); CHECK_STR(a, "1.500000e+000"); }
TC_CASE(large_exp_both)
{
    vs(0, a, sizeof a, "%e", d1e100); vs(0x10, c, sizeof c, "%e", d1e100);
    CHECK_STR(a, "1.000000e+100"); CHECK_STR(c, "1.000000e+100");
}
TC_CASE(upper_and_g)
{
    vs(0x10, a, sizeof a, "%E|%.0e|%g", d15, d1, d1e5);
    CHECK_STR(a, "1.500000E+000|1e+000|1e-005");
    vs(0, a, sizeof a, "%g", d1e5);
    CHECK_STR(a, "1e-05");
}
int main(void)
{
    tc_begin();
    TC_RUN(two_digit_default); TC_RUN(three_digit_opt); TC_RUN(large_exp_both);
    TC_RUN(upper_and_g);
    return tc_end();
}
