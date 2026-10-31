/* TC
 * id:    mf_copysignf_basic
 * area:  math_float
 * funcs: _copysignf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _copysign, _copysignf
 */
#include "tc.h"
#include <math.h>
static volatile float p3 = 3, m3 = -3, p0 = 0.0f, m0 = -0.0f, one = 1, m1 = -1, v_inf, v_nan, v_mnan;
static void init(void) { union { unsigned int u; float f; } x; x.u = 0x7F800000u; v_inf = x.f; x.u = 0x7FC00000u; v_nan = x.f; x.u = 0xFFC00000u; v_mnan = x.f; }

TC_CASE(signs) { CHECK_BITS_F(_copysignf(p3, m1), -3.0f); CHECK_BITS_F(_copysignf(m3, one), 3.0f); }
TC_CASE(zeros) { CHECK_BITS_F(_copysignf(p0, m0), -0.0f); CHECK_BITS_F(_copysignf(m0, p0), 0.0f); }
TC_CASE(inf) { CHECK_BITS_F(_copysignf(v_inf, m1), -v_inf); }
TC_CASE(nan_sign) { float r = _copysignf(v_nan, m1); CHECK(r != r); CHECK((tc_fbits(r) & 0x80000000u) != 0); }
TC_CASE(sign_from_nan) { CHECK_BITS_F(_copysignf(one, v_mnan), -1.0f); }
int main(void) { tc_begin(); init(); TC_RUN(signs); TC_RUN(zeros); TC_RUN(inf); TC_RUN(nan_sign); TC_RUN(sign_from_nan); return tc_end(); }
