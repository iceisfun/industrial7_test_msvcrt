/* TC
 * id:    gcy_neg_cexp_overflow_nonfinite
 * area:  math_c99
 * funcs: 
 * procs: cexp
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 G.6.3.1; MS docs cexp
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

TC_CASE(overflow_real) { cplx r = call("cexp", 1000.0, 0.0); CHECK(r.re == PINF()); CHECK_BITS_D(r.im, 0.0); }
TC_CASE(overflow_complex) { cplx r = call("cexp", 1000.0, 1.0); CHECK(r.re == PINF()); CHECK(r.im == PINF()); }
TC_CASE(zero_plus_i_inf) { cplx r = call("cexp", 0.0, PINF()); CHECK_IS_NAN(r.re); CHECK_IS_NAN(r.im); }
TC_CASE(inf_inf) { cplx r = call("cexp", PINF(), PINF()); CHECK(r.re == PINF() || r.re == -PINF()); CHECK_IS_NAN(r.im); }
TC_CASE(nan_one) { cplx r = call("cexp", QNAN(), 1.0); CHECK_IS_NAN(r.re); CHECK_IS_NAN(r.im); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(overflow_real); TC_RUN(overflow_complex); TC_RUN(zero_plus_i_inf); TC_RUN(inf_inf); TC_RUN(nan_one); CHECK_EQ_I(tc_iph_count, 0); return tc_end(); }
