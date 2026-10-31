/* TC
 * id:    mf_neg_powf_inf_operands
 * area:  math_float
 * funcs:
 * procs: powf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.4.4; MS docs pow, powf
 * note:  ucrtbase only: pre-UCRT x64 pow infinity-operand handling not known to follow C99 Annex F.
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

TC_CASE(half_pow_pinf_zero) { volatile float x = 0.5f, y = v_inf; CHECK_BITS_F(powf(x, y), 0.0f); }
TC_CASE(half_pow_ninf_inf) { volatile float x = 0.5f, y = -v_inf; CHECK(powf(x, y) == v_inf); }
TC_CASE(two_pow_ninf_zero) { volatile float x = 2.0f, y = -v_inf; CHECK_BITS_F(powf(x, y), 0.0f); }
TC_CASE(pinf_pow_neg_zero) { volatile float x = v_inf, y = -0.0f; CHECK_BITS_F(powf(x, y), 1.0f); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(powf); if (!(tc_p_powf)) { tc_write("TC FAIL " TC_TEST_ID "::proc not exported\n"); return 1; }
    v_inf = v_one / v_zero; v_nan = fb(0x7fc00000);
    TC_RUN(half_pow_pinf_zero); TC_RUN(half_pow_ninf_inf); TC_RUN(two_pow_ninf_zero); TC_RUN(pinf_pow_neg_zero);
    return tc_end();
}
