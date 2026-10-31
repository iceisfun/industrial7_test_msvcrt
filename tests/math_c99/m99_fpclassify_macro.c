/* TC
 * id:    m99_fpclassify_macro
 * area:  math_c99
 * funcs: _dclass _fdclass
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.12.3; MS math.h fpclassify/isnan/isinf/isfinite/isnormal built on _dclass/_fdclass
 * note:  Local macros replicate the UCRT math.h definitions via _dclass/_fdclass (mingw macros would bypass the export); results are cross-checked against compiler builtins.
 */
#include "tc.h"
#include <math.h>
short __cdecl _dclass(double);
short __cdecl _fdclass(float);
#include <float.h>
#define M_NAN 2
#define M_INF 1
#define M_ZERO 0
#define M_NORM (-1)
#define M_SUB (-2)
#define cls(x) (sizeof(x) == sizeof(float) ? _fdclass((float)(x)) : _dclass((double)(x)))
#define m_isnan(x) (cls(x) == M_NAN)
#define m_isinf(x) (cls(x) == M_INF)
#define m_isfinite(x) (cls(x) <= 0)
#define m_isnormal(x) (cls(x) == M_NORM)
static double mkd(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(classes) { volatile double a = 1.0, b = 0.0, c = DBL_TRUE_MIN, d = mkd(0x7ff0000000000000ULL), e = mkd(0x7ff8000000000000ULL);
    volatile float fa = 1.0f, fb = FLT_TRUE_MIN, fc = (float)e;
    CHECK_EQ_I(cls(a), M_NORM); CHECK_EQ_I(cls(b), M_ZERO); CHECK_EQ_I(cls(c), M_SUB); CHECK_EQ_I(cls(d), M_INF); CHECK_EQ_I(cls(e), M_NAN);
    CHECK_EQ_I(_fdclass(fa), M_NORM); CHECK_EQ_I(_fdclass(fb), M_SUB); CHECK_EQ_I(_fdclass(fc), M_NAN); }
TC_CASE(isnan_m) { volatile double n = mkd(0x7ff8000000000000ULL), one = 1.0; volatile float fn = (float)n;
    CHECK(m_isnan(n)); CHECK(!m_isnan(one)); CHECK(m_isnan(fn)); CHECK(!m_isnan(1.0f)); }
TC_CASE(isinf_m) { volatile double i = mkd(0x7ff0000000000000ULL), ni = mkd(0xfff0000000000000ULL), mx = DBL_MAX;
    CHECK(m_isinf(i)); CHECK(m_isinf(ni)); CHECK(!m_isinf(mx)); }
TC_CASE(isfinite_m) { volatile double one = 1.0, i = mkd(0x7ff0000000000000ULL), n = mkd(0x7ff8000000000000ULL), z = 0.0, s = DBL_TRUE_MIN;
    CHECK(m_isfinite(one)); CHECK(!m_isfinite(i)); CHECK(!m_isfinite(n)); CHECK(m_isfinite(z)); CHECK(m_isfinite(s)); }
TC_CASE(isnormal_m) { volatile double one = 1.0, z = 0.0, s = DBL_TRUE_MIN, i = mkd(0x7ff0000000000000ULL);
    CHECK(m_isnormal(one)); CHECK(!m_isnormal(z)); CHECK(!m_isnormal(s)); CHECK(!m_isnormal(i)); }
TC_CASE(agrees_with_builtins) { static const unsigned long long bits[] = { 0, 0x8000000000000000ULL, 1, 0x000fffffffffffffULL, 0x0010000000000000ULL, 0x3ff0000000000000ULL,
    0x7fefffffffffffffULL, 0x7ff0000000000000ULL, 0xfff0000000000000ULL, 0x7ff8000000000000ULL, 0xfff8000000000001ULL }; int i;
    for (i = 0; i < (int)(sizeof bits / sizeof bits[0]); i++) { volatile double d = mkd(bits[i]);
        CHECK_EQ_I(m_isnan(d) != 0, __builtin_isnan(d) != 0); CHECK_EQ_I(m_isinf(d) != 0, __builtin_isinf(d) != 0);
        CHECK_EQ_I(m_isfinite(d) != 0, __builtin_isfinite(d) != 0); CHECK_EQ_I(m_isnormal(d) != 0, __builtin_isnormal(d) != 0); } }
int main(void) { tc_begin(); TC_RUN(classes); TC_RUN(isnan_m); TC_RUN(isinf_m); TC_RUN(isfinite_m); TC_RUN(isnormal_m); TC_RUN(agrees_with_builtins); return tc_end(); }
