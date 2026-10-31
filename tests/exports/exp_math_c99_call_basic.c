/* TC
 * id:    exp_math_c99_call_basic
 * area:  exports
 * funcs: round trunc cbrt fmax fmin log2 expm1
 * procs: copysign hypot
 * crt:   msvcr120 ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12 / F.10; MS docs round, cbrt, fmax, log2, hypot
 */
#include "tc.h"
#include <math.h>

static volatile double v_zero = 0.0;
TC_DECL_PROC(double, copysign, (double, double));
TC_DECL_PROC(double, hypot, (double, double));

TC_CASE(round_half_away) { volatile double a = 2.5, b = -2.5, c = 0.4; CHECK_BITS_D(round(a), 3.0); CHECK_BITS_D(round(b), -3.0); CHECK_BITS_D(round(c), 0.0); }
TC_CASE(trunc_toward_zero) { volatile double a = -1.9, b = 1.9; CHECK_BITS_D(trunc(a), -1.0); CHECK_BITS_D(trunc(b), 1.0); }
TC_CASE(cbrt_27) { volatile double a = 27.0, b = -8.0; CHECK_NEAR(cbrt(a), 3.0, 1e-12); CHECK_NEAR(cbrt(b), -2.0, 1e-12); }
TC_CASE(fmax_fmin_with_nan)
{
    volatile double n = v_zero / v_zero, one = 1.0, two = 2.0;
    CHECK_BITS_D(fmax(n, one), 1.0); CHECK_BITS_D(fmax(one, n), 1.0);
    CHECK_BITS_D(fmin(n, one), 1.0); CHECK_BITS_D(fmin(one, n), 1.0);
    CHECK_BITS_D(fmax(one, two), 2.0); CHECK_BITS_D(fmin(one, two), 1.0);
}
TC_CASE(copysign_neg) { volatile double a = 3.0, b = -0.0; if (!tc_p_copysign) TC_SKIP("not exported"); CHECK_BITS_D(tc_p_copysign(a, b), -3.0); CHECK_BITS_D(tc_p_copysign(-a, 1.0), 3.0); }
TC_CASE(log2_8) { volatile double a = 8.0, b = 1.0; CHECK_NEAR(log2(a), 3.0, 1e-12); CHECK_BITS_D(log2(b), 0.0); }
TC_CASE(expm1_zero) { volatile double a = 0.0; CHECK_BITS_D(expm1(a), 0.0); }
TC_CASE(hypot_3_4) { volatile double a = 3.0, b = 4.0; if (!tc_p_hypot) TC_SKIP("not exported"); CHECK_NEAR(tc_p_hypot(a, b), 5.0, 1e-12); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(copysign); TC_LOAD_PROC(hypot);
    TC_RUN(round_half_away); TC_RUN(trunc_toward_zero); TC_RUN(cbrt_27); TC_RUN(fmax_fmin_with_nan);
    TC_RUN(copysign_neg); TC_RUN(log2_8); TC_RUN(expm1_zero); TC_RUN(hypot_3_4);
    return tc_end();
}
