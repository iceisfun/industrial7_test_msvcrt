/* TC
 * id:    pff_neg_snprintf_c99_precision_trunc
 * area:  stdio_printf
 * kind:  negative
 * funcs: snprintf vsnprintf _snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.5 snprintf / 7.21.6.12 vsnprintf (returns would-be length, always NUL-terminates when n>0, nothing written when n==0); MS docs _snprintf (legacy: -1 on truncation)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vsn(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vsnprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(prec100_into_8)
{
    char b[8]; volatile double v = 1.0; int r;
    r = snprintf(b, 8, "%.100f", v);
    CHECK_EQ_I(r, 102); CHECK_STR(b, "1.00000");
}
TC_CASE(prec30e_into_1)
{
    char b[2] = { 'x', 'y' }; volatile double v = 1.0; int r;
    r = snprintf(b, 1, "%.30e", v);
    CHECK_EQ_I(r, 36); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 'y');
}
TC_CASE(length_query_prec512)
{
    volatile double v = 0.1; int r;
    r = snprintf(NULL, 0, "%.512f", v);
    CHECK_EQ_I(r, 514);
}
TC_CASE(size0_untouched)
{
    char b[4] = { 'a', 'b', 'c', 0 }; volatile double v = 1.0; int r;
    r = snprintf(b, 0, "%f", v);
    CHECK_EQ_I(r, 8); CHECK_STR(b, "abc");
}
TC_CASE(vsnprintf_same)
{
    char b[8], c[2] = { 'x', 'y' }, d[4] = { 'a', 'b', 'c', 0 }; int r;
    r = vsn(b, 8, "%.100f", 1.0); CHECK_EQ_I(r, 102); CHECK_STR(b, "1.00000");
    r = vsn(c, 1, "%.30e", 1.0);  CHECK_EQ_I(r, 36); CHECK_EQ_I(c[0], 0); CHECK_EQ_I(c[1], 'y');
    r = vsn(NULL, 0, "%.512f", 0.1); CHECK_EQ_I(r, 514);
    r = vsn(d, 0, "%f", 1.0); CHECK_EQ_I(r, 8); CHECK_STR(d, "abc");
}
TC_CASE(legacy_underscore_snprintf_minus1)
{
    char b[16]; volatile double v = 1.0; int r, i;
    for (i = 0; i < 16; i++) b[i] = 'x';
    r = _snprintf(b, 8, "%.100f", v);
    CHECK_EQ_I(r, -1);
    CHECK(strncmp(b, "1.000000", 8) == 0);
    CHECK_EQ_I(b[8], 'x');
}
int main(void)
{
    tc_begin();
    TC_RUN(prec100_into_8); TC_RUN(prec30e_into_1); TC_RUN(length_query_prec512); TC_RUN(size0_untouched); TC_RUN(vsnprintf_same); TC_RUN(legacy_underscore_snprintf_minus1);
    return tc_end();
}
