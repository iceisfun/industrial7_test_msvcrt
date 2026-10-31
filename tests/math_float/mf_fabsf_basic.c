/* TC
 * id:    mf_fabsf_basic
 * area:  math_float
 * funcs:
 * procs: fabsf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.4.3 (7.12.7.2); MS docs fabs, fabsf
 * note:  fabsf is resolved at run time (export presence varies by CRT); skipped if not exported.
 */
#include "tc.h"
#include <float.h>

typedef float (__cdecl *fabsf_t)(float);
static fabsf_t p_fabsf;
static volatile float v_zero = 0.0f, v_one = 1.0f;

TC_CASE(zero) { volatile float n = -0.0f; if (!p_fabsf) TC_SKIP("fabsf not exported"); CHECK_BITS_F(p_fabsf(n), 0.0f); }
TC_CASE(values) { volatile float a = -3.5f, b = 3.5f; if (!p_fabsf) TC_SKIP("fabsf not exported"); CHECK_BITS_F(p_fabsf(a), 3.5f); CHECK_BITS_F(p_fabsf(b), 3.5f); }
TC_CASE(inf_nan)
{
    volatile float i = -(v_one / v_zero), n = v_zero / v_zero; float r;
    if (!p_fabsf) TC_SKIP("fabsf not exported");
    CHECK_BITS_F(p_fabsf(i), v_one / v_zero);
    r = p_fabsf(n); CHECK(r != r);
}
TC_CASE(denorm) { volatile float d = -FLT_MIN / 2.0f; if (!p_fabsf) TC_SKIP("fabsf not exported"); CHECK_BITS_F(p_fabsf(d), FLT_MIN / 2.0f); CHECK_EQ_U(tc_fbits(p_fabsf(d)), 0x00400000u); }

int main(void)
{
    tc_begin(); p_fabsf = (fabsf_t)tc_crt_proc("fabsf");
    TC_RUN(zero); TC_RUN(values); TC_RUN(inf_nan); TC_RUN(denorm);
    return tc_end();
}
