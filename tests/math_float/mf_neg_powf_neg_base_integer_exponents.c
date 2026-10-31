/* TC
 * id:    mf_neg_powf_neg_base_integer_exponents
 * area:  math_float
 * funcs:
 * procs: powf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.4.4, 7.12.7.4; MS docs pow
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static float fb(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
#define SENT 1234
TC_DECL_PROC(float, powf, (float, float));
#undef powf
#define powf tc_p_powf

TC_CASE(m2_pow_3) { volatile float x = -2.0f, y = 3.0f; CHECK_BITS_F(powf(x, y), -8.0f); }
TC_CASE(m2_pow_2) { volatile float x = -2.0f, y = 2.0f; CHECK_BITS_F(powf(x, y), 4.0f); }
TC_CASE(m2_pow_m3) { volatile float x = -2.0f, y = -3.0f; CHECK_BITS_F(powf(x, y), -0.125f); }
TC_CASE(m8_pow_one_third_nan) { volatile float x = -8.0f, y = 1.0f / 3.0f; CHECK_IS_NAN(powf(x, y)); }
TC_CASE(m2_pow_2p5_nan) { volatile float x = -2.0f, y = 2.5f; CHECK_IS_NAN(powf(x, y)); }
TC_CASE(m2_pow_large_even_float_inf) { volatile float x = -2.0f, y = 200.0f; CHECK(powf(x, y) == v_inf); }
TC_CASE(m2_pow_large_odd_float_minf) { volatile float x = -2.0f, y = 201.0f; CHECK(powf(x, y) == -v_inf); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(powf); if (!(tc_p_powf)) { tc_write("TC FAIL " TC_TEST_ID "::proc not exported\n"); return 1; }
    v_inf = v_one / v_zero;
    TC_RUN(m2_pow_3); TC_RUN(m2_pow_2); TC_RUN(m2_pow_m3); TC_RUN(m8_pow_one_third_nan); TC_RUN(m2_pow_2p5_nan);
    TC_RUN(m2_pow_large_even_float_inf); TC_RUN(m2_pow_large_odd_float_minf);
    return tc_end();
}
