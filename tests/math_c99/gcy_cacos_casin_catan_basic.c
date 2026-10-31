/* TC
 * id:    gcy_cacos_casin_catan_basic
 * area:  math_c99
 * funcs: 
 * procs: cacos casin catan
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.3.5.1-3, G.6.1
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

TC_CASE(cacos_pts) { cplx a = call("cacos", 1.0, 0.0), b = call("cacos", 0.0, 0.0), c = call("cacos", -1.0, 0.0);
    CHECK(isz(a.re)); CHECK(isz(a.im)); CHECK_NEAR(b.re, PI / 2, 1e-15); CHECK(isz(b.im)); CHECK_NEAR(c.re, PI, 1e-15); CHECK(isz(c.im)); }
TC_CASE(casin_zero) { cplx a = call("casin", 0.0, 0.0), b = call("casin", v_nzero, 0.0); CHECK(isz(a.re)); CHECK(isz(a.im)); CHECK(isz(b.re)); CHECK(isz(b.im)); }
TC_CASE(casin_one) { cplx a = call("casin", 1.0, 0.0); CHECK_NEAR(a.re, PI / 2, 1e-15); CHECK(isz(a.im)); }
TC_CASE(catan_pts) { cplx a = call("catan", 0.0, 0.0), b = call("catan", 1.0, 0.0); CHECK(isz(a.re)); CHECK(isz(a.im)); CHECK_NEAR(b.re, PI / 4, 1e-15); CHECK(isz(b.im)); }
TC_CASE(casin_real) { cplx a = call("casin", 0.5, 0.0); CHECK_NEAR(a.re, 0.5235987755982989, 1e-14); CHECK(isz(a.im)); }
TC_CASE(sum_halfpi) { cplx a = call("cacos", 0.3, 0.4), b = call("casin", 0.3, 0.4); CHECK_NEAR(a.re + b.re, PI / 2, 1e-14); CHECK_NEAR(a.im + b.im, 0.0, 1e-14); }

int main(void) { tc_begin(); TC_RUN(cacos_pts); TC_RUN(casin_zero); TC_RUN(casin_one); TC_RUN(catan_pts); TC_RUN(casin_real); TC_RUN(sum_halfpi); return tc_end(); }
