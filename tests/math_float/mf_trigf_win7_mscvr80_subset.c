/* TC
 * id:    mf_trigf_win7_mscvr80_subset
 * area:  math_float
 * funcs:
 * procs: sinf cosf tanf sinhf coshf log10f modff fmodf
 * crt:   msvcr80 msvcr90
 * tier:  p2
 * conf:  documented
 * ref:   C11 7.12; MS docs sin/cos/tan/sinh/cosh/log10/modf/fmod (float overloads: sinf etc. in the x64 CRTs)
 * note:  basic f-functions on the oldest CRTs. Resolved dynamically because the f-variants exist as exports only in some builds; each case skips if missing. sqrtf/expf/logf/powf are not covered here.
 */
#include "tc.h"
TC_DECL_PROC(float, sinf, (float)); TC_DECL_PROC(float, cosf, (float)); TC_DECL_PROC(float, tanf, (float));
TC_DECL_PROC(float, sinhf, (float)); TC_DECL_PROC(float, coshf, (float)); TC_DECL_PROC(float, log10f, (float));
TC_DECL_PROC(float, modff, (float, float *)); TC_DECL_PROC(float, fmodf, (float, float));
static volatile float v0 = 0.0f, v100 = 100.0f, v55 = 5.5f, v2 = 2.0f, v375 = 3.75f, vm375 = -3.75f;

TC_CASE(sin_cos_zero)
{
    if (!tc_p_sinf || !tc_p_cosf) TC_SKIP("sinf/cosf not exported");
    CHECK_BITS_F(tc_p_sinf(v0), 0.0f); CHECK_BITS_F(tc_p_cosf(v0), 1.0f);
}
TC_CASE(tan_zero) { if (!tc_p_tanf) TC_SKIP("tanf not exported"); CHECK_BITS_F(tc_p_tanf(v0), 0.0f); }
TC_CASE(sinh_cosh_zero)
{
    if (!tc_p_sinhf || !tc_p_coshf) TC_SKIP("sinhf/coshf not exported");
    CHECK_BITS_F(tc_p_sinhf(v0), 0.0f); CHECK_BITS_F(tc_p_coshf(v0), 1.0f);
}
TC_CASE(log10_100) { if (!tc_p_log10f) TC_SKIP("log10f not exported"); CHECK_NEAR(tc_p_log10f(v100), 2.0, 1e-6); }
TC_CASE(fmod_basic) { if (!tc_p_fmodf) TC_SKIP("fmodf not exported"); CHECK_BITS_F(tc_p_fmodf(v55, v2), 1.5f); }
TC_CASE(modf_basic)
{
    float i = -1.0f, f;
    if (!tc_p_modff) TC_SKIP("modff not exported");
    f = tc_p_modff(v375, &i); CHECK_BITS_F(i, 3.0f); CHECK_BITS_F(f, 0.75f);
    f = tc_p_modff(vm375, &i); CHECK_BITS_F(i, -3.0f); CHECK_BITS_F(f, -0.75f);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(sinf); TC_LOAD_PROC(cosf); TC_LOAD_PROC(tanf); TC_LOAD_PROC(sinhf); TC_LOAD_PROC(coshf);
    TC_LOAD_PROC(log10f); TC_LOAD_PROC(modff); TC_LOAD_PROC(fmodf);
    TC_RUN(sin_cos_zero); TC_RUN(tan_zero); TC_RUN(sinh_cosh_zero); TC_RUN(log10_100); TC_RUN(fmod_basic); TC_RUN(modf_basic);
    return tc_end();
}
