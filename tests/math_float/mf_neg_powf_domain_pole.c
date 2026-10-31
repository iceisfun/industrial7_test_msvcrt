/* TC
 * id:    mf_neg_powf_domain_pole
 * area:  math_float
 * funcs:
 * procs: powf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.4.4; MS docs pow, powf
 * note:  domain errors yield NaN, poles of +0 yield +inf (signed -0 poles are in mf_neg_powf_negzero_probe), overflow/underflow yield inf/0
 */
#include "tc.h"
#include <math.h>

TC_DECL_PROC(float, powf, (float, float));
#define powf tc_p_powf

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;

TC_CASE(neg_base_frac_exp) { volatile float a = -2.0f, b = 0.5f, c = -8.0f, d = 1.0f / 3.0f; CHECK_IS_NAN(powf(a, b)); CHECK_IS_NAN(powf(c, d)); }
TC_CASE(pole_pos_zero) { volatile float m1 = -1.0f, m2 = -2.0f; CHECK(powf(v_zero, m1) == v_inf); CHECK(powf(v_zero, m2) == v_inf); }
TC_CASE(overflow) { volatile float t = 10.0f, e = 100.0f; CHECK(powf(t, e) == v_inf); }
TC_CASE(underflow) { volatile float t = 10.0f, e = -100.0f; CHECK_BITS_F(powf(t, e), 0.0f); }
TC_CASE(nan_in) { volatile float e = 1.0f; CHECK_IS_NAN(powf(v_nan, e)); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(powf); if (!tc_p_powf) { tc_write("TC FAIL " TC_TEST_ID "::powf not exported\n"); return 1; }
    v_inf = v_one / v_zero; v_nan = v_zero / v_zero;
    TC_RUN(neg_base_frac_exp); TC_RUN(pole_pos_zero); TC_RUN(overflow); TC_RUN(underflow); TC_RUN(nan_in);
    return tc_end();
}
