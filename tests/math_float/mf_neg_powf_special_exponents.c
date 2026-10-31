/* TC
 * id:    mf_neg_powf_special_exponents
 * area:  math_float
 * funcs:
 * procs: powf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.4.4; MS docs pow, powf
 * note:  ucrtbase only: pre-UCRT x64 pow special-case handling (nan^0, 1^nan, (-1)^inf) is not known to follow C99 Annex F.
 *        errno cases moved to mf_neg_powf_inf_errno_probe.
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

TC_CASE(nan_pow_zero_is_one) { volatile float x = v_nan, y = 0.0f, z = -0.0f; CHECK_BITS_F(powf(x, y), 1.0f); CHECK_BITS_F(powf(x, z), 1.0f); }
TC_CASE(one_pow_nan_is_one) { volatile float x = 1.0f, y = v_nan; CHECK_BITS_F(powf(x, y), 1.0f); }
TC_CASE(m1_pow_pinf_is_one) { volatile float x = -1.0f, y = v_inf; CHECK_BITS_F(powf(x, y), 1.0f); }
TC_CASE(m1_pow_ninf_is_one) { volatile float x = -1.0f, y = -v_inf; CHECK_BITS_F(powf(x, y), 1.0f); }
TC_CASE(ninf_pow_odd_int) { volatile float x = -v_inf, y = 3.0f; CHECK(powf(x, y) == -v_inf); }
TC_CASE(ninf_pow_even_int) { volatile float x = -v_inf, y = 2.0f; CHECK(powf(x, y) == v_inf); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(powf); if (!(tc_p_powf)) { tc_write("TC FAIL " TC_TEST_ID "::proc not exported\n"); return 1; }
    v_inf = v_one / v_zero; v_nan = fb(0x7fc00000);
    TC_RUN(nan_pow_zero_is_one); TC_RUN(one_pow_nan_is_one); TC_RUN(m1_pow_pinf_is_one); TC_RUN(m1_pow_ninf_is_one);
   
    TC_RUN(ninf_pow_odd_int); TC_RUN(ninf_pow_even_int);
    return tc_end();
}
