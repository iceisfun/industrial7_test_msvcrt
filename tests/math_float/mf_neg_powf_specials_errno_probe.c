/* TC
 * id:    mf_neg_powf_specials_errno_probe
 * area:  math_float
 * funcs:
 * procs: powf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.4.4; MS docs pow
 * note:  probes errno of powf: EDOM for negative base with non-integer exponent, ERANGE on underflow/overflow,
 *        and no errno change for infinity operands. Windows errno policy for float pow is not documented per case.
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

TC_CASE(neg_base_frac_edom_cube_root) { volatile float x = -8.0f, y = 1.0f / 3.0f, r; errno = 0; r = powf(x, y); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(underflow_erange) { volatile float x = 2.0f, y = -200.0f, r; errno = 0; r = powf(x, y); CHECK_BITS_F(r, 0.0f); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(pos_inf_base_errno_unchanged) { volatile float x = v_inf, y = 2.0f, r; errno = SENT; r = powf(x, y); CHECK(r == v_inf); CHECK_EQ_I(errno, SENT); }
TC_CASE(m1_pow_inf_errno_unchanged) { volatile float x = -1.0f, y = v_inf, r; errno = SENT; r = powf(x, y); CHECK_BITS_F(r, 1.0f); CHECK_EQ_I(errno, SENT); }
TC_CASE(neg_base_huge_odd_int_overflow_sign) { volatile float x = -2.0f, y = 201.0f, r; errno = 0; r = powf(x, y); CHECK(r == -v_inf); CHECK_EQ_I(errno, ERANGE); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(powf); if (!(tc_p_powf)) { tc_write("TC FAIL " TC_TEST_ID "::proc not exported\n"); return 1; }
    v_inf = v_one / v_zero; v_nan = fb(0x7fc00000);
    TC_RUN(neg_base_frac_edom_cube_root); TC_RUN(underflow_erange); TC_RUN(pos_inf_base_errno_unchanged);
    TC_RUN(m1_pow_inf_errno_unchanged); TC_RUN(neg_base_huge_odd_int_overflow_sign);
    return tc_end();
}
