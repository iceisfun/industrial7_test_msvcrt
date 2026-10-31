/* TC
 * id:    mf_neg_copysignf_nan_inf_zero
 * area:  math_float
 * funcs: _copysignf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _copysign, copysign; C11 7.12.11.1
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static float fb(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
#define SENT 1234

TC_CASE(nan_magnitude_neg_sign_bit_set) { volatile float m = fb(0x7fc00000u), s = -1.0f; CHECK_EQ_U(tc_fbits(_copysignf(m, s)) >> 31, 1); }
TC_CASE(inf_magnitude_neg_sign) { volatile float m = v_inf, s = -2.0f; CHECK_BITS_F(_copysignf(m, s), fb(0xff800000u)); }
TC_CASE(zero_magnitude_neg_sign) { volatile float m = 0.0f, s = -2.0f; CHECK_BITS_F(_copysignf(m, s), -0.0f); }
TC_CASE(sign_source_neg_nan) { volatile float m = 1.0f, s = fb(0xffc00000u); CHECK_BITS_F(_copysignf(m, s), -1.0f); }
TC_CASE(sign_source_neg_zero) { volatile float m = 3.0f, s = -0.0f; CHECK_BITS_F(_copysignf(m, s), -3.0f); }
TC_CASE(errno_unchanged) { volatile float m = v_inf, s = -1.0f, r; errno = SENT; r = _copysignf(m, s); r = _copysignf(fb(0x7fc00000u), s); (void)r; CHECK_EQ_I(errno, SENT); }

int main(void)
{
    tc_begin(); v_inf = v_one / v_zero;
    TC_RUN(nan_magnitude_neg_sign_bit_set); TC_RUN(inf_magnitude_neg_sign); TC_RUN(zero_magnitude_neg_sign);
    TC_RUN(sign_source_neg_nan); TC_RUN(sign_source_neg_zero); TC_RUN(errno_unchanged);
    return tc_end();
}
