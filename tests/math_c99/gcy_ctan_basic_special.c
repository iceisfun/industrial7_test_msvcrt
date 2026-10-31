/* TC
 * id:    gcy_ctan_basic_special
 * area:  math_c99
 * funcs: 
 * procs: ctan
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.3.5.6, G.6.2.6
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

TC_CASE(zeros) { cplx a = call("ctan", 0.0, 0.0), b = call("ctan", v_nzero, 0.0); CHECK(isz(a.re)); CHECK(isz(a.im)); CHECK(isz(b.re)); CHECK(isz(b.im)); }
TC_CASE(i) { cplx a = call("ctan", 0.0, 1.0); CHECK(isz(a.re)); CHECK_NEAR(a.im, 0.7615941559557649, 1e-14); }
TC_CASE(real_one) { cplx a = call("ctan", 1.0, 0.0); CHECK_NEAR(a.re, 1.5574077246549023, 1e-14); CHECK(isz(a.im)); }
TC_CASE(y_large) { cplx a = call("ctan", 1.0, 1000.0); CHECK_NEAR(a.im, 1.0, 1e-14); CHECK_NEAR(a.re, 0.0, 1e-14); }

int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(i); TC_RUN(real_one); TC_RUN(y_large); return tc_end(); }
