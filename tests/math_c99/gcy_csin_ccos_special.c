/* TC
 * id:    gcy_csin_ccos_special
 * area:  math_c99
 * funcs: 
 * procs: csin ccos
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  probe
 * ref:   C11 G.6.2.4-5 (csin(z)=-i csinh(iz), ccos(z)=ccosh(iz))
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

TC_CASE(csin_zero_iinf) { cplx a = call("csin", 0.0, PINF()); CHECK(isz(a.re)); CHECK(a.im == PINF()); }
TC_CASE(csin_inf_zero) { cplx a = call("csin", PINF(), 0.0); CHECK_IS_NAN(a.re); CHECK(isz(a.im)); }
TC_CASE(ccos_zero_iinf) { cplx a = call("ccos", 0.0, PINF()); CHECK(a.re == PINF()); CHECK(isz(a.im)); }
TC_CASE(ccos_nan_zero) { cplx a = call("ccos", QNAN(), 0.0); CHECK_IS_NAN(a.re); CHECK(isz(a.im)); }
TC_CASE(csin_nan_zero) { cplx a = call("csin", QNAN(), 0.0); CHECK_IS_NAN(a.re); CHECK(isz(a.im)); }

int main(void) { tc_begin(); TC_RUN(csin_zero_iinf); TC_RUN(csin_inf_zero); TC_RUN(ccos_zero_iinf); TC_RUN(ccos_nan_zero); TC_RUN(csin_nan_zero); return tc_end(); }
