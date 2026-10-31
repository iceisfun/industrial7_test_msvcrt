/* TC
 * id:    mf_neg_ldexpf_extreme_exp_probe
 * area:  math_float
 * funcs:
 * procs: ldexpf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.6.6, F.10.3.6
 * note:  PROBE: ldexpf with exp == INT_MAX / INT_MIN (no int overflow inside the CRT), +-0 with huge exp keeps sign,
 *        and smallest subnormal * 2^-1 (exact tie) rounds to even = +0 in round-to-nearest.
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

TC_CASE(zero_exp_huge_keeps_sign)
{
    volatile float z = 0.0f, nz = -0.0f; volatile int e = INT_MAX;
    NEED(tc_p_ldexpf);
    CHECK_BITS_F(tc_p_ldexpf(z, e), 0.0f); CHECK_BITS_F(tc_p_ldexpf(nz, e), -0.0f);
    e = INT_MIN; CHECK_BITS_F(tc_p_ldexpf(z, e), 0.0f); CHECK_BITS_F(tc_p_ldexpf(nz, e), -0.0f);
}
TC_CASE(int_max_exp_inf) { volatile float x = 1.0f, y = -1.0f; volatile int e = INT_MAX; NEED(tc_p_ldexpf); CHECK_BITS_F(tc_p_ldexpf(x, e), F_INF()); CHECK_BITS_F(tc_p_ldexpf(y, e), -F_INF()); }
TC_CASE(int_min_exp_zero_signed) { volatile float x = 1.0f, y = -1.0f; volatile int e = INT_MIN; NEED(tc_p_ldexpf); CHECK_BITS_F(tc_p_ldexpf(x, e), 0.0f); CHECK_BITS_F(tc_p_ldexpf(y, e), -0.0f); }
TC_CASE(subnormal_tie_rounds_to_zero) { volatile float d = 1.4012985e-45f; volatile int e = -1; NEED(tc_p_ldexpf); CHECK_BITS_F(tc_p_ldexpf(d, e), 0.0f); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(ldexpf);
    TC_RUN(zero_exp_huge_keeps_sign); TC_RUN(int_max_exp_inf); TC_RUN(int_min_exp_zero_signed); TC_RUN(subnormal_tie_rounds_to_zero);
    return tc_end();
}
