/* TC
 * id:    gcy_csinh_ccosh_ctanh
 * area:  math_c99
 * funcs: 
 * procs: csinh ccosh ctanh
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 G.6.2.4/.5/.6
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

TC_CASE(zeros) { cplx a = call("csinh", 0.0, 0.0), b = call("csinh", v_nzero, 0.0), c = call("ccosh", 0.0, 0.0), d = call("ctanh", 0.0, 0.0);
    CHECK(isz(a.re)); CHECK(isz(a.im)); CHECK(isz(b.re)); CHECK(isz(b.im));
    CHECK_BITS_D(c.re, 1.0); CHECK(isz(c.im)); CHECK(isz(d.re)); CHECK(isz(d.im)); }
TC_CASE(real_one) { cplx a = call("csinh", 1.0, 0.0), b = call("ccosh", 1.0, 0.0), c = call("ctanh", 1.0, 0.0);
    CHECK_NEAR(a.re, 1.1752011936438014, 1e-14); CHECK_NEAR(b.re, 1.5430806348152437, 1e-14); CHECK_NEAR(c.re, 0.7615941559557649, 1e-14);
    CHECK(isz(a.im)); CHECK(isz(b.im)); CHECK(isz(c.im)); }

int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(real_one); return tc_end(); }
