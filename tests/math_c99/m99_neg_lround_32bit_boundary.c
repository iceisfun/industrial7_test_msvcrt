/* TC
 * id:    m99_neg_lround_32bit_boundary
 * area:  math_c99
 * funcs: lround lroundf llround llroundf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs lround/llround (long is 32-bit on Windows, long long 64-bit); C11 7.12.9.7
 * note:  LLP64: lround range is 32-bit, llround must not clamp to LONG range.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>
#include <limits.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }
static double d_snan(void) { union { unsigned long long u; double d; } x; x.u = 0x7ff0000000000001ULL; return x.d; }


TC_CASE(sizes) { CHECK_EQ_I(sizeof(long), 4); CHECK_EQ_I(sizeof(long long), 8); }
TC_CASE(lround_in_range) { volatile double a=2147483647.4, b=-2147483648.4; CHECK_EQ_I(lround(a), 2147483647); CHECK_EQ_I(lround(b), -2147483647-1); }
TC_CASE(llround_beyond_long) { volatile double a=2147483648.0, b=-2147483649.0;
    CHECK(llround(a) == 2147483648LL); CHECK(llround(b) == -2147483649LL); }
TC_CASE(llround_exact) { volatile double a=4503599627370497.0, b=9007199254740992.0, c=9007199254740994.0;
    CHECK(llround(a) == 4503599627370497LL); CHECK(llround(b) == 9007199254740992LL); CHECK(llround(c) == 9007199254740994LL); }
TC_CASE(float_variants) { volatile float a=16777216.0f, b=2147483648.0f;
    CHECK_EQ_I(lroundf(a), 16777216); CHECK(llroundf(b) == 2147483648LL); }
int main(void) { tc_begin(); TC_RUN(sizes); TC_RUN(lround_in_range); TC_RUN(llround_beyond_long); TC_RUN(llround_exact); TC_RUN(float_variants); return tc_end(); }
