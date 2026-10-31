/* TC
 * id:    mf_floorf_ceilf_special
 * area:  math_float
 * funcs: floorf ceilf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.6.1, F.10.6.2; MS docs floor, ceil
 */
#include "tc.h"
#include <math.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;

TC_CASE(neg_zero) { volatile float n = -0.0f, p = 0.0f; CHECK_BITS_F(floorf(n), -0.0f); CHECK_BITS_F(ceilf(n), -0.0f); CHECK_BITS_F(floorf(p), 0.0f); CHECK_BITS_F(ceilf(p), 0.0f); }
TC_CASE(ceil_neg_frac) { volatile float x = -0.5f; CHECK_BITS_F(ceilf(x), -0.0f); }
TC_CASE(inf) { volatile float p = v_inf, n = -v_inf; CHECK_BITS_F(floorf(p), v_inf); CHECK_BITS_F(floorf(n), -v_inf); CHECK_BITS_F(ceilf(p), v_inf); CHECK_BITS_F(ceilf(n), -v_inf); }
TC_CASE(nan) { CHECK_IS_NAN(floorf(v_nan)); CHECK_IS_NAN(ceilf(v_nan)); }
TC_CASE(huge) { volatile float h = 1e30f, m = -1e30f; CHECK_BITS_F(floorf(h), 1e30f); CHECK_BITS_F(ceilf(h), 1e30f); CHECK_BITS_F(floorf(m), -1e30f); CHECK_BITS_F(ceilf(m), -1e30f); }

int main(void)
{
    tc_begin(); v_inf = v_one / v_zero; v_nan = v_zero / v_zero;
    TC_RUN(neg_zero); TC_RUN(ceil_neg_frac); TC_RUN(inf); TC_RUN(nan); TC_RUN(huge);
    return tc_end();
}
