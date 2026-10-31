/* TC
 * id:    mf_neg_powf_inf_errno_probe
 * area:  math_float
 * funcs:
 * procs: powf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.4.4; MS docs pow
 * note:  probes that powf with exact/no-error special operands (0.5^inf, 2^-inf, inf^-0, nan^0, 1^nan, (-inf)^3)
 *        leaves errno untouched. Standard implies no error, but Windows errno policy is not documented per case.
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

TC_CASE(errno_unchanged_inf_exps) { volatile float h = 0.5f, t = 2.0f, i = v_inf, r; errno = SENT; r = powf(h, i); r = powf(t, -i); r = powf(i, -0.0f); (void)r; CHECK_EQ_I(errno, SENT); }
TC_CASE(errno_unchanged_nan_one) { volatile float x = v_nan, y = 0.0f, one = 1.0f, nn = v_nan, r; errno = SENT; r = powf(x, y); r = powf(one, nn); (void)r; CHECK_EQ_I(errno, SENT); }
TC_CASE(errno_unchanged_ninf_odd) { volatile float x = -v_inf, y = 3.0f, r; errno = SENT; r = powf(x, y); (void)r; CHECK_EQ_I(errno, SENT); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(powf); if (!(tc_p_powf)) { tc_write("TC FAIL " TC_TEST_ID "::proc not exported\n"); return 1; }
    v_inf = v_one / v_zero; v_nan = fb(0x7fc00000);
    TC_RUN(errno_unchanged_inf_exps); TC_RUN(errno_unchanged_nan_one); TC_RUN(errno_unchanged_ninf_odd);
    return tc_end();
}
