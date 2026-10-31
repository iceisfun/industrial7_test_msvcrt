/* TC
 * id:    gcy_cacosh_casinh_catanh_basic
 * area:  math_c99
 * funcs: 
 * procs: cacosh casinh catanh
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.3.6.1-3, G.6.2.1-3
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

TC_CASE(cacosh_pts) { cplx a = call("cacosh", 1.0, 0.0), b = call("cacosh", 0.0, 0.0), c = call("cacosh", -1.0, 0.0);
    CHECK(isz(a.re)); CHECK(isz(a.im)); CHECK(isz(b.re)); CHECK_NEAR(b.im, PI / 2, 1e-15); CHECK(isz(c.re)); CHECK_NEAR(c.im, PI, 1e-15); }
TC_CASE(casinh_zero) { cplx a = call("casinh", 0.0, 0.0), b = call("casinh", v_nzero, 0.0); CHECK(isz(a.re)); CHECK(isz(a.im)); CHECK(isz(b.re)); CHECK(isz(b.im)); }
TC_CASE(casinh_i) { cplx a = call("casinh", 0.0, 1.0); CHECK(isz(a.re)); CHECK_NEAR(a.im, PI / 2, 1e-15); }
TC_CASE(catanh_zero) { cplx a = call("catanh", 0.0, 0.0); CHECK(isz(a.re)); CHECK(isz(a.im)); }
TC_CASE(catanh_half) { cplx a = call("catanh", 0.5, 0.0); CHECK_NEAR(a.re, 0.5493061443340548, 1e-14); CHECK(isz(a.im)); }
TC_CASE(cacosh_two) { cplx a = call("cacosh", 2.0, 0.0); CHECK_NEAR(a.re, 1.3169578969248166, 1e-14); CHECK(isz(a.im)); }

int main(void) { tc_begin(); TC_RUN(cacosh_pts); TC_RUN(casinh_zero); TC_RUN(casinh_i); TC_RUN(catanh_zero); TC_RUN(catanh_half); TC_RUN(cacosh_two); return tc_end(); }
