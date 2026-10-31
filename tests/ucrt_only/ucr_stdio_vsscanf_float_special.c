/* TC
 * id:    ucr_stdio_vsscanf_float_special
 * area:  ucrt_only
 * funcs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C99 7.21.6.2 / 7.22.1.3 (strtod syntax: inf, infinity, nan); UCRT supports these since VS2015
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <errno.h>

static int vsc(unsigned long long opts, const char *in, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsscanf(opts, in, (size_t)-1, f, NULL, ap); va_end(ap);
    return r;
}

static double mk_(unsigned long long b) { union { double d; unsigned long long u; } x; x.u = b; return x.d; }
static double pinf(void) { return mk_(0x7ff0000000000000ULL); }
TC_CASE(inf) { double d = 0; CHECK_EQ_I(vsc(0, "inf", "%lf", &d), 1); CHECK_BITS_D(d, pinf()); }
TC_CASE(neg_inf_upper) { double d = 0; CHECK_EQ_I(vsc(0, "-INF", "%lf", &d), 1); CHECK_BITS_D(d, -pinf()); }
TC_CASE(infinity) { double d = 0; CHECK_EQ_I(vsc(0, "infinity", "%lf", &d), 1); CHECK_BITS_D(d, pinf()); }
TC_CASE(nan) { double d = 0; CHECK_EQ_I(vsc(0, "nan", "%lf", &d), 1); CHECK_IS_NAN(d); }
TC_CASE(float_inf) { float f = 0; CHECK_EQ_I(vsc(0, "inf", "%f", &f), 1); CHECK(f > 3.4e38f && f == f * 1.0f && f + 1.0f == f); }
TC_CASE(exp) { double d = 0; CHECK_EQ_I(vsc(0, "1e3", "%lf", &d), 1); CHECK_BITS_D(d, 1000.0); }

int main(void)
{
    tc_begin();
    TC_RUN(inf);
    TC_RUN(neg_inf_upper);
    TC_RUN(infinity);
    TC_RUN(nan);
    TC_RUN(float_inf);
    TC_RUN(exp);
    return tc_end();
}
