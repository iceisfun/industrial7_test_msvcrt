/* TC
 * id:    mf_neg_l_variants_errors
 * area:  math_float
 * funcs:
 * procs: logl sqrtl powl asinl fmodl
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   C11 F.10.1.2 asin, F.10.4.4 pow, F.10.4.5 sqrt, F.10.3.7 log, F.10.7.1 fmod; MS docs asin, log, pow, sqrt, fmod
 * note:  Domain/pole errors of the l-variants (double ABI) mirror the double functions. A case skips if the
 *        l-name is not exported by that CRT (presence is probed in mf_l_variants_exports_probe).
 */
#include "tc.h"

typedef double (__cdecl *d1_t)(double);
typedef double (__cdecl *d2_t)(double, double);
static d1_t logl_, sqrtl_, asinl_; static d2_t powl_, fmodl_;
static volatile double v_m1 = -1.0, v_0 = 0.0, v_1 = 1.0, v_2 = 2.0, v_inf;

TC_CASE(logl_neg) { if (!logl_) TC_SKIP("logl missing"); CHECK_IS_NAN(logl_(v_m1)); }
TC_CASE(logl_zero) { if (!logl_) TC_SKIP("logl missing"); CHECK_BITS_D(logl_(v_0), -v_inf); }
TC_CASE(sqrtl_neg) { if (!sqrtl_) TC_SKIP("sqrtl missing"); CHECK_IS_NAN(sqrtl_(v_m1)); }
TC_CASE(powl_pole) { if (!powl_) TC_SKIP("powl missing"); CHECK_BITS_D(powl_(v_0, v_m1), v_inf); }
TC_CASE(asinl_range) { if (!asinl_) TC_SKIP("asinl missing"); CHECK_IS_NAN(asinl_(v_2)); }
TC_CASE(fmodl_zero) { if (!fmodl_) TC_SKIP("fmodl missing"); CHECK_IS_NAN(fmodl_(v_1, v_0)); }

int main(void)
{
    tc_begin();
    logl_ = (d1_t)tc_crt_proc("logl"); sqrtl_ = (d1_t)tc_crt_proc("sqrtl"); asinl_ = (d1_t)tc_crt_proc("asinl");
    powl_ = (d2_t)tc_crt_proc("powl"); fmodl_ = (d2_t)tc_crt_proc("fmodl");
    v_inf = v_1 / v_0;
    TC_RUN(logl_neg); TC_RUN(logl_zero); TC_RUN(sqrtl_neg); TC_RUN(powl_pole); TC_RUN(asinl_range); TC_RUN(fmodl_zero);
    return tc_end();
}
