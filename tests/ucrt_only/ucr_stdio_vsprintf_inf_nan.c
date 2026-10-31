/* TC
 * id:    ucr_stdio_vsprintf_inf_nan
 * area:  ucrt_only
 * funcs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.21.6.1 (f,F,e,E: infinity/NaN; "inf"/"infinity" and case are as C says, exact spelling inf is the UCRT choice)
 * note:  Width/sign flags applied to inf/nan and precision being ignored are UCRT behaviour from experience, not cited.
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

TC_CASE(lower) { char b[64]; vs(0, b, sizeof b, "%f|%f", dv(0x7ff0000000000000ULL), dv(0xfff0000000000000ULL)); CHECK_STR(b, "inf|-inf"); }
TC_CASE(upper) { char b[64]; vs(0, b, sizeof b, "%F|%E|%F", dv(0x7ff0000000000000ULL), dv(0x7ff0000000000000ULL), dv(0xfff0000000000000ULL)); CHECK_STR(b, "INF|INF|-INF"); }
TC_CASE(nan_lower) { char b[64]; vs(0, b, sizeof b, "%f|%e|%g", dv(0x7ff8000000000000ULL), dv(0x7ff8000000000000ULL), dv(0x7ff8000000000000ULL)); CHECK_STR(b, "nan|nan|nan"); }
TC_CASE(nan_upper) { char b[64]; vs(0, b, sizeof b, "%F|%E|%G", dv(0x7ff8000000000000ULL), dv(0x7ff8000000000000ULL), dv(0x7ff8000000000000ULL)); CHECK_STR(b, "NAN|NAN|NAN"); }
TC_CASE(width) { char b[64]; vs(0, b, sizeof b, "[%8f][%-8f][%+f][% f]", dv(0x7ff0000000000000ULL), dv(0x7ff0000000000000ULL), dv(0x7ff0000000000000ULL), dv(0x7ff0000000000000ULL)); CHECK_STR(b, "[     inf][inf     ][+inf][ inf]"); }
TC_CASE(ignores_precision) { char b[64]; vs(0, b, sizeof b, "%.2f|%.0e", dv(0x7ff0000000000000ULL), dv(0x7ff0000000000000ULL)); CHECK_STR(b, "inf|inf"); }

int main(void)
{
    tc_begin();
    TC_RUN(lower);
    TC_RUN(upper);
    TC_RUN(nan_lower);
    TC_RUN(nan_upper);
    TC_RUN(width);
    TC_RUN(ignores_precision);
    return tc_end();
}
