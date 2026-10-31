/* TC
 * id:    mf_l_variants_exp_log_pow_sqrt
 * area:  math_float
 * funcs:
 * procs: expl logl log10l powl sqrtl copysignl exp log log10 pow sqrt
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  inferred
 * ref:   MS docs "Long double" (long double == double); exp, log, log10, pow, sqrt, copysign
 * note:  conf inferred: docs say long double == double; bit-identical results between the l-name and the double
 *        name assume the l-name is an alias/thunk of it (expected, not documented).
 *        Called through a double signature. A case skips if the l-name is not exported (header inline only).
 */
#include "tc.h"

typedef double (__cdecl *d1_t)(double);
typedef double (__cdecl *d2_t)(double, double);
static d1_t expl_, logl_, log10l_, sqrtl_, exp_, log_, log10_, sqrt_;
static d2_t powl_, pow_, copysignl_;
static volatile double v_1 = 1.0, v_2 = 2.0, v_1000 = 1000.0, v_10 = 10.0, v_nz = -0.0;

TC_CASE(expl) { if (!expl_) TC_SKIP("expl not exported"); CHECK_BITS_D(expl_(v_1), exp_(v_1)); CHECK_BITS_D(expl_(v_2), exp_(v_2)); CHECK_BITS_D(expl_(0.0), 1.0); }
TC_CASE(logl) { if (!logl_) TC_SKIP("logl not exported"); CHECK_BITS_D(logl_(v_2), log_(v_2)); CHECK_BITS_D(logl_(v_1), 0.0); }
TC_CASE(log10l) { if (!log10l_) TC_SKIP("log10l not exported"); CHECK_BITS_D(log10l_(v_1000), log10_(v_1000)); CHECK_NEAR(log10l_(v_1000), 3.0, 1e-14); }
TC_CASE(powl) { if (!powl_) TC_SKIP("powl not exported"); CHECK_BITS_D(powl_(v_2, v_10), 1024.0); CHECK_BITS_D(powl_(v_2, 0.5), pow_(v_2, 0.5)); }
TC_CASE(sqrtl) { if (!sqrtl_) TC_SKIP("sqrtl not exported"); CHECK_BITS_D(sqrtl_(v_2), sqrt_(v_2)); CHECK_BITS_D(sqrtl_(4.0), 2.0); }
TC_CASE(copysignl) { if (!copysignl_) TC_SKIP("copysignl not exported"); CHECK_BITS_D(copysignl_(v_1, v_nz), -1.0); CHECK_BITS_D(copysignl_(-3.0, 0.0), 3.0); }

int main(void)
{
    tc_begin();
    expl_ = (d1_t)tc_crt_proc("expl"); logl_ = (d1_t)tc_crt_proc("logl"); log10l_ = (d1_t)tc_crt_proc("log10l");
    sqrtl_ = (d1_t)tc_crt_proc("sqrtl"); powl_ = (d2_t)tc_crt_proc("powl"); copysignl_ = (d2_t)tc_crt_proc("copysignl");
    exp_ = (d1_t)tc_crt_proc("exp"); log_ = (d1_t)tc_crt_proc("log"); log10_ = (d1_t)tc_crt_proc("log10");
    sqrt_ = (d1_t)tc_crt_proc("sqrt"); pow_ = (d2_t)tc_crt_proc("pow");
    if (!exp_ || !log_ || !log10_ || !sqrt_ || !pow_) { tc_write("TC FAIL " TC_TEST_ID "::double proc missing\n"); return 1; }
    TC_RUN(expl); TC_RUN(logl); TC_RUN(log10l); TC_RUN(powl); TC_RUN(sqrtl); TC_RUN(copysignl);
    return tc_end();
}
