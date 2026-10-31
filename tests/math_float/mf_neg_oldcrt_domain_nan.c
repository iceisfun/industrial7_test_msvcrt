/* TC
 * id:    mf_neg_oldcrt_domain_nan
 * area:  math_float
 * funcs:
 * procs: sinf fmodf log10f
 * crt:   msvcr80 msvcr90 msvcr100
 * tier:  p2
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.1.6 sin(inf) invalid -> NaN; F.10.7.1 fmod(x,0) -> NaN; F.10.3.2 log10(x<0) -> NaN; MS docs sin, fmod, log10
 * note:  sign of NaN is not asserted. Math functions do not call the invalid-parameter handler. Skips if not exported.
 */
#include "tc.h"
TC_DECL_PROC(float, sinf, (float)); TC_DECL_PROC(float, fmodf, (float, float)); TC_DECL_PROC(float, log10f, (float));
static volatile float v_zero = 0.0f, v_one = 1.0f, v_m1 = -1.0f, v_inf;

TC_CASE(sinf_inf) { if (!tc_p_sinf) TC_SKIP("sinf not exported"); CHECK_IS_NAN(tc_p_sinf(v_inf)); CHECK_IS_NAN(tc_p_sinf(-v_inf)); }
TC_CASE(fmodf_zero) { if (!tc_p_fmodf) TC_SKIP("fmodf not exported"); CHECK_IS_NAN(tc_p_fmodf(v_one, v_zero)); }
TC_CASE(log10f_neg) { if (!tc_p_log10f) TC_SKIP("log10f not exported"); CHECK_IS_NAN(tc_p_log10f(v_m1)); }
TC_CASE(no_iph) { CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); v_inf = v_one / v_zero;
    tc_iph_install();
    TC_LOAD_PROC(sinf); TC_LOAD_PROC(fmodf); TC_LOAD_PROC(log10f);
    TC_RUN(sinf_inf); TC_RUN(fmodf_zero); TC_RUN(log10f_neg); TC_RUN(no_iph);
    return tc_end();
}
