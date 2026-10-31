/* TC
 * id:    mf_neg_float_denormal_underflow
 * area:  math_float
 * funcs:
 * procs: expf powf sqrtf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   C11 F.10.3.1 exp, F.10.4.4 pow, F.10.4.5 sqrt, 7.12.1 (underflow); float.h FLT_MIN 1.17549435e-38f
 * note:  conf inferred: correct rounding of exp/pow far below the smallest denormal is expected quality, not mandated.
 *        Results far below FLT_MIN. Asserts only what holds with or without denormal support: result <= FLT_MIN
 *        and >= 0, and results far below the smallest denormal (1.4e-45) are exactly +0. Exact denormal values
 *        are probed in mf_neg_float_denormal_exact_probe.
 */
#include "tc.h"

#define FLT_MIN_V 1.17549435e-38f
typedef float (__cdecl *f1_t)(float);
typedef float (__cdecl *f2_t)(float, float);
static f1_t p_expf, p_sqrtf; static f2_t p_powf;
static volatile float v_m90 = -90.0f, v_m110 = -110.0f, v_m1000 = -1000.0f, v_2 = 2.0f, v_m160 = -160.0f, v_m200 = -200.0f, v_0 = 0.0f;

TC_CASE(expf_m90_small) { float r = p_expf(v_m90); CHECK(r >= 0.0f); CHECK(r <= FLT_MIN_V); }
TC_CASE(expf_m110_zero) { CHECK_BITS_F(p_expf(v_m110), 0.0f); }
TC_CASE(expf_m1000_zero) { CHECK_BITS_F(p_expf(v_m1000), 0.0f); }
TC_CASE(powf_2_m160_zero) { CHECK_BITS_F(p_powf(v_2, v_m160), 0.0f); }
TC_CASE(powf_2_m200_zero) { CHECK_BITS_F(p_powf(v_2, v_m200), 0.0f); }
TC_CASE(sqrtf_zero_exact) { CHECK_BITS_F(p_sqrtf(v_0), 0.0f); }

int main(void)
{
    tc_begin();
    p_expf = (f1_t)tc_crt_proc("expf"); p_sqrtf = (f1_t)tc_crt_proc("sqrtf"); p_powf = (f2_t)tc_crt_proc("powf");
    if (!p_expf || !p_sqrtf || !p_powf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; }
    TC_RUN(expf_m90_small); TC_RUN(expf_m110_zero); TC_RUN(expf_m1000_zero); TC_RUN(powf_2_m160_zero); TC_RUN(powf_2_m200_zero); TC_RUN(sqrtf_zero_exact);
    return tc_end();
}
