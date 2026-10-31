/* TC
 * id:    m99_fpclassify_dclass
 * area:  math_c99
 * funcs: _dclass _fdclass
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  observed
 * ref:   MS math.h FP_NAN 2, FP_INFINITE 1, FP_ZERO 0, FP_NORMAL -1, FP_SUBNORMAL -2 (UCRT fpclassify implementation); C11 7.12.3.1
 * note:  Constants hard-coded to the Microsoft math.h values, NOT mingw's FP_* (which differ).
 */
#include "tc.h"
#include <math.h>
short __cdecl _dclass(double);
short __cdecl _fdclass(float);
#include <float.h>
#define MS_FP_NAN 2
#define MS_FP_INFINITE 1
#define MS_FP_ZERO 0
#define MS_FP_NORMAL (-1)
#define MS_FP_SUBNORMAL (-2)
static double mkd(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static float mkf(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
TC_CASE(zero) { volatile double a = 0.0, b = -0.0; CHECK_EQ_I(_dclass(a), MS_FP_ZERO); CHECK_EQ_I(_dclass(b), MS_FP_ZERO); }
TC_CASE(normal) { volatile double a = 1.0, b = -123.5, c = DBL_MAX, d = DBL_MIN; CHECK_EQ_I(_dclass(a), MS_FP_NORMAL); CHECK_EQ_I(_dclass(b), MS_FP_NORMAL);
    CHECK_EQ_I(_dclass(c), MS_FP_NORMAL); CHECK_EQ_I(_dclass(d), MS_FP_NORMAL); }
TC_CASE(subnormal) { volatile double a = mkd(1ULL), b = mkd(0x800fffffffffffffULL); CHECK_EQ_I(_dclass(a), MS_FP_SUBNORMAL); CHECK_EQ_I(_dclass(b), MS_FP_SUBNORMAL); }
TC_CASE(infinite) { volatile double a = mkd(0x7ff0000000000000ULL), b = mkd(0xfff0000000000000ULL); CHECK_EQ_I(_dclass(a), MS_FP_INFINITE); CHECK_EQ_I(_dclass(b), MS_FP_INFINITE); }
TC_CASE(nan) { volatile double a = mkd(0x7ff8000000000000ULL), b = mkd(0xfff8000000000000ULL), c = mkd(0x7ff0000000000001ULL); CHECK_EQ_I(_dclass(a), MS_FP_NAN); CHECK_EQ_I(_dclass(b), MS_FP_NAN); CHECK_EQ_I(_dclass(c), MS_FP_NAN); }
TC_CASE(float_all) { volatile float z = 0.0f, nz = -0.0f, n = 1.5f, s = mkf(1u), i = mkf(0x7f800000u), ni = mkf(0xff800000u), q = mkf(0x7fc00000u), mx = FLT_MAX, mn = FLT_MIN;
    CHECK_EQ_I(_fdclass(z), MS_FP_ZERO); CHECK_EQ_I(_fdclass(nz), MS_FP_ZERO); CHECK_EQ_I(_fdclass(n), MS_FP_NORMAL); CHECK_EQ_I(_fdclass(mx), MS_FP_NORMAL); CHECK_EQ_I(_fdclass(mn), MS_FP_NORMAL);
    CHECK_EQ_I(_fdclass(s), MS_FP_SUBNORMAL); CHECK_EQ_I(_fdclass(i), MS_FP_INFINITE); CHECK_EQ_I(_fdclass(ni), MS_FP_INFINITE); CHECK_EQ_I(_fdclass(q), MS_FP_NAN); }
int main(void) { tc_begin(); TC_RUN(zero); TC_RUN(normal); TC_RUN(subnormal); TC_RUN(infinite); TC_RUN(nan); TC_RUN(float_all); return tc_end(); }
