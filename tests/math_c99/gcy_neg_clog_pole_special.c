/* TC
 * id:    gcy_neg_clog_pole_special
 * area:  math_c99
 * funcs: 
 * procs: clog
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 G.6.3.2
 * note:  PROBE: Annex G special values. Microsoft does not define __STDC_IEC_559_COMPLEX__, so Annex G conformance of the MS complex.h functions is not guaranteed; real Windows decides.
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

TC_CASE(pole_signs) {
    cplx a = call("clog", v_nzero, 0.0), b = call("clog", 0.0, 0.0), c = call("clog", v_nzero, v_nzero), d = call("clog", 0.0, v_nzero);
    CHECK(a.re == -PINF()); CHECK_NEAR(a.im, PI, 1e-15);
    CHECK(b.re == -PINF()); CHECK_BITS_D(b.im, 0.0);
    CHECK(c.re == -PINF()); CHECK_NEAR(c.im, -PI, 1e-15);
    CHECK(d.re == -PINF()); CHECK_BITS_D(d.im, -0.0); }
TC_CASE(y_inf) { cplx a = call("clog", 2.0, PINF()), b = call("clog", -2.0, PINF()); CHECK(a.re == PINF()); CHECK_NEAR(a.im, PI / 2, 1e-15); CHECK(b.re == PINF()); CHECK_NEAR(b.im, PI / 2, 1e-15); }
TC_CASE(nan_nan) { cplx a = call("clog", QNAN(), QNAN()); CHECK_IS_NAN(a.re); CHECK_IS_NAN(a.im); }
TC_CASE(inf_nan) { cplx a = call("clog", PINF(), QNAN()); CHECK(a.re == PINF()); CHECK_IS_NAN(a.im); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(pole_signs); TC_RUN(y_inf); TC_RUN(nan_nan); TC_RUN(inf_nan); CHECK_EQ_I(tc_iph_count, 0); return tc_end(); }
