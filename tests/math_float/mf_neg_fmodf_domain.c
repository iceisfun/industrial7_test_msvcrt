/* TC
 * id:    mf_neg_fmodf_domain
 * area:  math_float
 * funcs: fmodf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.7.1; MS docs fmod, fmodf
 */
#include "tc.h"
#include <math.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;

TC_CASE(zero_divisor) { volatile float x = 1.0f, p = 0.0f, n = -0.0f; CHECK_IS_NAN(fmodf(x, p)); CHECK_IS_NAN(fmodf(x, n)); }
TC_CASE(inf_dividend) { volatile float y = 2.0f, a = v_inf, b = -v_inf; CHECK_IS_NAN(fmodf(a, y)); CHECK_IS_NAN(fmodf(b, y)); }
TC_CASE(nan_args) { volatile float two = 2.0f; CHECK_IS_NAN(fmodf(v_nan, two)); CHECK_IS_NAN(fmodf(two, v_nan)); }

int main(void)
{
    tc_begin(); v_inf = v_one / v_zero; v_nan = v_zero / v_zero;
    TC_RUN(zero_divisor); TC_RUN(inf_dividend); TC_RUN(nan_args);
    return tc_end();
}
