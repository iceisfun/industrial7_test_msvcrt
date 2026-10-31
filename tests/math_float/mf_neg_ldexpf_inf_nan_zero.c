/* TC
 * id:    mf_neg_ldexpf_inf_nan_zero
 * area:  math_float
 * funcs:
 * procs: ldexpf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.6.6 / F.10.3.6; MS docs ldexp
 * note:  extreme exponents (INT_MAX/INT_MIN) and the subnormal tie case are in mf_neg_ldexpf_extreme_exp_probe: old CRTs add exp to the stored exponent in int arithmetic.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>
#include <limits.h>

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }
#define SENT 0x5a5
#define NEED(p) do { if (!(p)) TC_SKIP("function not exported"); } while (0)

TC_DECL_PROC(float, ldexpf, (float, int));

TC_CASE(inf_exp_neg) { volatile float x = F_INF(), y = -F_INF(); volatile int e = -1000; NEED(tc_p_ldexpf); CHECK_BITS_F(tc_p_ldexpf(x, e), F_INF()); CHECK_BITS_F(tc_p_ldexpf(y, e), -F_INF()); }
TC_CASE(nan_exp_pos) { volatile float x = F_NAN(); volatile int e = 1000; NEED(tc_p_ldexpf); CHECK_IS_NAN(tc_p_ldexpf(x, e)); }
TC_CASE(zero_keeps_sign_moderate_exp)
{
    volatile float z = 0.0f, nz = -0.0f; volatile int e = 100;
    NEED(tc_p_ldexpf);
    CHECK_BITS_F(tc_p_ldexpf(z, e), 0.0f); CHECK_BITS_F(tc_p_ldexpf(nz, e), -0.0f);
}
TC_CASE(overflow_to_inf) { volatile float x = 1.0f, y = -1.0f; volatile int e = 200; NEED(tc_p_ldexpf); CHECK_BITS_F(tc_p_ldexpf(x, e), F_INF()); CHECK_BITS_F(tc_p_ldexpf(y, e), -F_INF()); }
TC_CASE(denormal_result_to_zero_via_neg_exp)
{
    volatile float x = 1.0f, y = -1.0f; volatile int e = -200;
    NEED(tc_p_ldexpf);
    CHECK_BITS_F(tc_p_ldexpf(x, e), 0.0f); CHECK_BITS_F(tc_p_ldexpf(y, e), -0.0f);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(ldexpf);
    TC_RUN(inf_exp_neg); TC_RUN(nan_exp_pos); TC_RUN(zero_keeps_sign_moderate_exp); TC_RUN(overflow_to_inf); TC_RUN(denormal_result_to_zero_via_neg_exp);
    return tc_end();
}
