/* TC
 * id:    mf_neg_msvcrt_x64_f_exports_probe
 * area:  math_float
 * funcs:
 * procs: sinf cosf sqrtf powf expf logf
 * crt:   msvcrt msvcr80 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sin/cos/sqrt/pow/exp/log (float overloads)
 * note:  PROBE: whether msvcrt.dll / msvcr80 / msvcr90 export the f-suffixed names at all. Absence is a legitimate outcome; the log lines (TC SKIP) record presence. Only asserts that IF sinf is present, sinf(0)==0 and cosf(0)==1 and sqrtf(4)==2 (if present).
 */
#include "tc.h"
TC_DECL_PROC(float, sinf, (float)); TC_DECL_PROC(float, cosf, (float)); TC_DECL_PROC(float, sqrtf, (float));
TC_DECL_PROC(float, powf, (float, float)); TC_DECL_PROC(float, expf, (float)); TC_DECL_PROC(float, logf, (float));
static volatile float v0 = 0.0f, v4 = 4.0f;

TC_CASE(presence_log)
{
    tc_write("TC INFO " TC_TEST_ID " sinf="); tc_write(tc_p_sinf ? "yes" : "no");
    tc_write(" cosf="); tc_write(tc_p_cosf ? "yes" : "no");
    tc_write(" sqrtf="); tc_write(tc_p_sqrtf ? "yes" : "no");
    tc_write(" powf="); tc_write(tc_p_powf ? "yes" : "no");
    tc_write(" expf="); tc_write(tc_p_expf ? "yes" : "no");
    tc_write(" logf="); tc_write(tc_p_logf ? "yes" : "no"); tc_write("\n");
}
TC_CASE(sinf_cosf_if_present)
{
    if (!tc_p_sinf || !tc_p_cosf) TC_SKIP("sinf/cosf absent");
    CHECK_BITS_F(tc_p_sinf(v0), 0.0f); CHECK_BITS_F(tc_p_cosf(v0), 1.0f);
}
TC_CASE(sqrtf_if_present) { if (!tc_p_sqrtf) TC_SKIP("sqrtf absent"); CHECK_BITS_F(tc_p_sqrtf(v4), 2.0f); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(sinf); TC_LOAD_PROC(cosf); TC_LOAD_PROC(sqrtf); TC_LOAD_PROC(powf); TC_LOAD_PROC(expf); TC_LOAD_PROC(logf);
    TC_RUN(presence_log); TC_RUN(sinf_cosf_if_present); TC_RUN(sqrtf_if_present);
    return tc_end();
}
