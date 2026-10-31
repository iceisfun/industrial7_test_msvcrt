/* TC
 * id:    gcy_neg_catanh_catan_poles
 * area:  math_c99
 * funcs: 
 * procs: catanh catan
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 G.6.2.3, G.6.1.3
 * note:  PROBE: Annex G special values. Microsoft does not define __STDC_IEC_559_COMPLEX__, so Annex G conformance of the MS complex.h functions is not guaranteed; real Windows decides. Return values and "invalid-parameter handler not invoked" asserted; errno is not asserted for pole inputs.
 */
#include "tc.h"
#include <math.h>
typedef struct { double re, im; } cplx;   /* MS _Dcomplex layout: passed/returned by hidden pointer on x64 */
typedef cplx (__cdecl *cfn_t)(cplx);
static volatile double v_zero = 0.0, v_one = 1.0, v_nzero = -0.0;
static double PINF(void) { return v_one / v_zero; }
static double QNAN(void) { return PINF() - PINF(); }
static double PI = 3.14159265358979323846;
static cplx C(double re, double im) { cplx z; z.re = re; z.im = im; return z; }
static cplx call(const char *name, double re, double im)
{
    cfn_t f = (cfn_t)tc_crt_proc(name);
    cplx z = C(re, im), r = C(0, 0);
    if (!f) { tc_fail_expr(__FILE__, __LINE__, "export missing"); return r; }
    return f(z);
}
static int isz(double d) { return d == 0.0; }
static int neg(double d) { return tc_dbits(d) >> 63; }

TC_CASE(catanh_plus1) { cplx a = call("catanh", 1.0, 0.0); CHECK(a.re == PINF()); CHECK_BITS_D(a.im, 0.0); }
TC_CASE(catanh_minus1) { cplx a = call("catanh", -1.0, 0.0); CHECK(a.re == -PINF()); CHECK_BITS_D(a.im, 0.0); }
TC_CASE(catanh_conj) { cplx a = call("catanh", 1.0, v_nzero); CHECK(a.re == PINF()); CHECK_BITS_D(a.im, -0.0); }
TC_CASE(catanh_nan) { cplx a = call("catanh", QNAN(), 0.0); CHECK_IS_NAN(a.re); CHECK_IS_NAN(a.im); }
TC_CASE(catanh_y_inf) { cplx a = call("catanh", 0.5, PINF()); CHECK_BITS_D(a.re, 0.0); CHECK_NEAR(a.im, PI / 2, 1e-15); }
TC_CASE(catan_pole_i) { cplx a = call("catan", 0.0, 1.0), b = call("catan", 0.0, -1.0); CHECK(isz(a.re)); CHECK(a.im == PINF()); CHECK(isz(b.re)); CHECK(b.im == -PINF()); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(catanh_plus1); TC_RUN(catanh_minus1); TC_RUN(catanh_conj); TC_RUN(catanh_nan); TC_RUN(catanh_y_inf); TC_RUN(catan_pole_i); CHECK_EQ_I(tc_iph_count, 0); return tc_end(); }
