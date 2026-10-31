/* TC
 * id:    mf_neg_powf_negzero_probe
 * area:  math_float
 * funcs:
 * procs: powf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.4.4
 * note:  PROBE: sign of the infinity from powf(-0, negative integer). C11 F.10.4.4 requires -inf for odd and +inf for even,
 *        but older Windows CRTs are not known to follow it; each case isolated so one wrong guess does not mask the other.
 */
#include "tc.h"
#include <math.h>

TC_DECL_PROC(float, powf, (float, float));
#define powf tc_p_powf

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf;

TC_CASE(odd) { volatile float nz = -0.0f, m1 = -1.0f; CHECK(powf(nz, m1) == -v_inf); }
TC_CASE(even) { volatile float nz = -0.0f, m2 = -2.0f; CHECK(powf(nz, m2) == v_inf); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(powf); if (!tc_p_powf) { tc_write("TC FAIL " TC_TEST_ID "::powf not exported\n"); return 1; }
    v_inf = v_one / v_zero;
    TC_RUN(odd); TC_RUN(even);
    return tc_end();
}
