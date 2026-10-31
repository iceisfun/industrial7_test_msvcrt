/* TC
 * id:    ucr_stdio_vsprintf_hexfloat_prec
 * area:  ucrt_only
 * funcs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 a,A conversion with precision; MS docs printf type a/A
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static int vs(unsigned long long opts, char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf(opts, b, n, f, NULL, ap); va_end(ap);
    return r;
}
static double mk(unsigned long long bits) { union { double d; unsigned long long u; } x; x.u = bits; return x.d; }
static volatile unsigned long long v_bits;
static double dv(unsigned long long bits) { v_bits = bits; return mk(v_bits); }

TC_CASE(p1_one) { char b[64]; vs(0, b, sizeof b, "%.1a", dv(0x3ff0000000000000ULL)); CHECK_STR(b, "0x1.0p+0"); }
TC_CASE(p0_two) { char b[64]; vs(0, b, sizeof b, "%.0a", dv(0x4000000000000000ULL)); CHECK_STR(b, "0x1p+1"); }
TC_CASE(upper) { char b[64]; vs(0, b, sizeof b, "%.3A", dv(0x406fe00000000000ULL)); CHECK_STR(b, "0X1.FE0P+7"); }
TC_CASE(zero) { char b[64]; vs(0, b, sizeof b, "%.2a", dv(0)); CHECK_STR(b, "0x0.00p+0"); }
TC_CASE(neg_p2) { char b[64]; vs(0, b, sizeof b, "%.2a", dv(0xbff8000000000000ULL)); CHECK_STR(b, "-0x1.80p+0"); }

int main(void)
{
    tc_begin();
    TC_RUN(p1_one);
    TC_RUN(p0_two);
    TC_RUN(upper);
    TC_RUN(zero);
    TC_RUN(neg_p2);
    return tc_end();
}
