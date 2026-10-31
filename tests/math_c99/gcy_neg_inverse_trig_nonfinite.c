/* TC
 * id:    gcy_neg_inverse_trig_nonfinite
 * area:  math_c99
 * funcs:
 * procs: cacos casin cacosh casinh
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.3.5.1-2, 7.3.6.1-2, G.6.1.1, G.6.1.2, G.6.2.1, G.6.2.2
 * note:  Arguments outside the real domain (|x|>1) are valid for complex inverse functions: no NaN, no errno/IPH.
 */
#include "tc.h"
#include <math.h>
typedef struct { double re, im; } cplx;   /* MS _Dcomplex: 16 bytes, hidden pointer on x64 */
typedef cplx (__cdecl *cfn_t)(cplx);
static volatile double v_zero = 0.0, v_one = 1.0;
static double PINF(void) { return v_one / v_zero; }
static double QNAN(void) { return PINF() - PINF(); }
static const double PI = 3.14159265358979323846;
static cplx C(double re, double im) { cplx z; z.re = re; z.im = im; return z; }
static cplx call(const char *name, double re, double im)
{
    cfn_t f = (cfn_t)tc_crt_proc(name);
    cplx z = C(re, im), r = C(0, 0);
    if (!f) { tc_fail_expr(__FILE__, __LINE__, "export missing"); return r; }
    return f(z);
}
static int isnan_(double d) { return d != d; }

static void iph(void) { tc_iph_install(); tc_iph_count = 0; }

TC_CASE(cacos_gt1_is_finite) { cplx r = call("cacos", 2.0, 0.0);
    CHECK(!isnan_(r.re)); CHECK_NEAR(r.re, 0.0, 1e-12); CHECK_NEAR(r.im, -1.3169578969248166, 1e-12); }
TC_CASE(casin_gt1_is_finite) { cplx r = call("casin", 2.0, 0.0);
    CHECK_NEAR(r.re, PI / 2, 1e-12); CHECK_NEAR(r.im, 1.3169578969248166, 1e-12); }
TC_CASE(casin_gt1_negzero_conj) { cplx r = call("casin", 2.0, -0.0);
    CHECK_NEAR(r.re, PI / 2, 1e-12); CHECK_NEAR(r.im, -1.3169578969248166, 1e-12); }
TC_CASE(nan_nan) { cplx a = call("cacos", QNAN(), QNAN()), b = call("cacosh", QNAN(), QNAN());
    CHECK(isnan_(a.re)); CHECK(isnan_(a.im)); CHECK(isnan_(b.re)); CHECK(isnan_(b.im)); }
TC_CASE(casin_inf_inf) { cplx r = call("casin", PINF(), PINF()); CHECK_NEAR(r.re, PI / 4, 1e-12); CHECK(r.im == PINF()); }
TC_CASE(cacosh_inf_inf) { cplx r = call("cacosh", PINF(), PINF()); CHECK(r.re == PINF()); CHECK_NEAR(r.im, PI / 4, 1e-12); }
TC_CASE(casinh_inf_nan) { cplx r = call("casinh", PINF(), QNAN()); CHECK(r.re == PINF() || r.re == -PINF()); CHECK(isnan_(r.im)); }
TC_CASE(iph_untouched) { iph();
    call("cacos", 2.0, 0.0); call("casin", 2.0, 0.0); call("cacosh", 0.5, 0.0); call("casinh", PINF(), QNAN());
    CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_RUN(cacos_gt1_is_finite); TC_RUN(casin_gt1_is_finite); TC_RUN(casin_gt1_negzero_conj); TC_RUN(nan_nan);
    TC_RUN(casin_inf_inf); TC_RUN(cacosh_inf_inf); TC_RUN(casinh_inf_nan); TC_RUN(iph_untouched); return tc_end(); }
