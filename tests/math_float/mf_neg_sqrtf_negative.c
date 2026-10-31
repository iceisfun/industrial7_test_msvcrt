/* TC
 * id:    mf_neg_sqrtf_negative
 * area:  math_float
 * funcs:
 * procs: sqrtf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 F.10.4.5; MS docs sqrt, sqrtf (EDOM, NaN)
 */
#include "tc.h"
#include <math.h>

TC_DECL_PROC(float, sqrtf, (float));
#define sqrtf tc_p_sqrtf
#include <errno.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;

TC_CASE(minus_one) { volatile float x = -1.0f; CHECK_IS_NAN(sqrtf(x)); }
TC_CASE(minus_inf) { volatile float x = -v_inf; CHECK_IS_NAN(sqrtf(x)); }
TC_CASE(tiny_negative) { volatile float x = -1e-30f; CHECK_IS_NAN(sqrtf(x)); }
TC_CASE(nan_in) { CHECK_IS_NAN(sqrtf(v_nan)); }
TC_CASE(errno_edom) { volatile float x = -1.0f; volatile float r; errno = 0; r = sqrtf(x); (void)r; CHECK_EQ_I(errno, EDOM); }
TC_CASE(no_iph)
{
    volatile float x = -1.0f; volatile float r;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; r = sqrtf(x); (void)r;
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(sqrtf); if (!tc_p_sqrtf) { tc_write("TC FAIL " TC_TEST_ID "::sqrtf not exported\n"); return 1; } v_inf = v_one / v_zero; v_nan = v_zero / v_zero;
    TC_RUN(minus_one); TC_RUN(minus_inf); TC_RUN(tiny_negative); TC_RUN(nan_in); TC_RUN(errno_edom); TC_RUN(no_iph);
    return tc_end();
}
