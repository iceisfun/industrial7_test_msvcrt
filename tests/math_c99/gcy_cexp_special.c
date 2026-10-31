/* TC
 * id:    gcy_cexp_special
 * area:  math_c99
 * funcs: 
 * procs: cexp
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  probe
 * ref:   C11 G.6.3.1
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

TC_CASE(signed_zero_imag) { cplx a = call("cexp", 0.0, 0.0), b = call("cexp", 0.0, v_nzero), c = call("cexp", v_nzero, 0.0), d = call("cexp", v_nzero, v_nzero);
    CHECK_BITS_D(a.re, 1.0); CHECK_BITS_D(a.im, 0.0); CHECK_BITS_D(b.re, 1.0); CHECK_BITS_D(b.im, -0.0);
    CHECK_BITS_D(c.re, 1.0); CHECK_BITS_D(c.im, 0.0); CHECK_BITS_D(d.re, 1.0); CHECK_BITS_D(d.im, -0.0); }
TC_CASE(neg_inf_finite_y) { /* +0 * cis(y): y=1 cos>0 sin>0; y=2 cos<0 sin>0 */
    cplx a = call("cexp", -PINF(), 1.0), b = call("cexp", -PINF(), 2.0), c = call("cexp", -PINF(), -1.0);
    CHECK_BITS_D(a.re, 0.0); CHECK_BITS_D(a.im, 0.0); CHECK_BITS_D(b.re, -0.0); CHECK_BITS_D(b.im, 0.0); CHECK_BITS_D(c.re, 0.0); CHECK_BITS_D(c.im, -0.0); }
TC_CASE(pos_inf_zero) { cplx a = call("cexp", PINF(), 0.0); CHECK(a.re == PINF()); CHECK_BITS_D(a.im, 0.0); }
TC_CASE(neg_inf_inf) { cplx a = call("cexp", -PINF(), PINF()); CHECK(isz(a.re)); CHECK(isz(a.im)); }
TC_CASE(nan_zero) { cplx a = call("cexp", QNAN(), 0.0); CHECK_IS_NAN(a.re); CHECK(isz(a.im)); CHECK(!neg(a.im)); }

int main(void) { tc_begin(); TC_RUN(signed_zero_imag); TC_RUN(neg_inf_finite_y); TC_RUN(pos_inf_zero); TC_RUN(neg_inf_inf); TC_RUN(nan_zero); return tc_end(); }
