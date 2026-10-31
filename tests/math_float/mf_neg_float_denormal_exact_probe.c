/* TC
 * id:    mf_neg_float_denormal_exact_probe
 * area:  math_float
 * funcs:
 * procs: powf sqrtf expf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   IEEE 754 binary32 (smallest denormal 2^-149, bits 0x00000001)
 * note:  PROBE: exact denormal handling (no flush-to-zero / DAZ) in float math: powf(2,-149) is bits 0x1;
 *        powf(2,-150) is an exact tie that round-to-nearest-even takes to +0; sqrtf(2^-149) > 0;
 *        expf(-90) is a nonzero denormal (about 8.2e-40).
 */
#include "tc.h"

typedef float (__cdecl *f1_t)(float);
typedef float (__cdecl *f2_t)(float, float);
static f1_t p_expf, p_sqrtf; static f2_t p_powf;
static volatile float v_2 = 2.0f, v_m149 = -149.0f, v_m150 = -150.0f, v_m90 = -90.0f;
static float from_bits(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
static volatile unsigned int v_minden = 1u;

TC_CASE(powf_min_denormal) { CHECK_BITS_F(p_powf(v_2, v_m149), from_bits(1u)); }
TC_CASE(powf_half_min_denormal_zero) { CHECK_BITS_F(p_powf(v_2, v_m150), 0.0f); }
TC_CASE(sqrtf_min_denormal_positive) { float r = p_sqrtf(from_bits(v_minden)); CHECK(r > 0.0f); CHECK_NEAR(r, 3.7431e-23, 1e-26); }
TC_CASE(expf_m90_denormal_nonzero) { float r = p_expf(v_m90); CHECK(r > 0.0f); CHECK(r < 1.17549435e-38f); }

int main(void)
{
    tc_begin();
    p_expf = (f1_t)tc_crt_proc("expf"); p_sqrtf = (f1_t)tc_crt_proc("sqrtf"); p_powf = (f2_t)tc_crt_proc("powf");
    if (!p_expf || !p_sqrtf || !p_powf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; }
    TC_RUN(powf_min_denormal); TC_RUN(powf_half_min_denormal_zero); TC_RUN(sqrtf_min_denormal_positive); TC_RUN(expf_m90_denormal_nonzero);
    return tc_end();
}
