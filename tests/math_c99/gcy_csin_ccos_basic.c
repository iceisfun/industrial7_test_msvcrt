/* TC
 * id:    gcy_csin_ccos_basic
 * area:  math_c99
 * funcs: 
 * procs: csin ccos
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.3.5.5/7.3.5.4, G.6.2
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

static cplx mul(cplx a, cplx b) { return C(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re); }
TC_CASE(csin_zero) { cplx a = call("csin", 0.0, 0.0), b = call("csin", v_nzero, 0.0); CHECK(isz(a.re)); CHECK(isz(a.im)); CHECK(isz(b.re)); CHECK(isz(b.im)); }
TC_CASE(csin_halfpi) { cplx a = call("csin", PI / 2, 0.0); CHECK_NEAR(a.re, 1.0, 1e-15); CHECK(isz(a.im)); }
TC_CASE(ccos_zero) { cplx a = call("ccos", 0.0, 0.0); CHECK_BITS_D(a.re, 1.0); CHECK(isz(a.im)); }
TC_CASE(ccos_i) { cplx a = call("ccos", 0.0, 1.0); CHECK_NEAR(a.re, 1.5430806348152437, 1e-14); CHECK(isz(a.im)); }
TC_CASE(csin_i) { cplx a = call("csin", 0.0, 1.0); CHECK(isz(a.re)); CHECK_NEAR(a.im, 1.1752011936438014, 1e-14); }
TC_CASE(csin_conj) { cplx a = call("csin", 1.0, 2.0), b = call("csin", 1.0, -2.0); CHECK_NEAR(a.re, b.re, 1e-14); CHECK_NEAR(a.im, -b.im, 1e-14); }
TC_CASE(pythagorean) { cplx s = call("csin", 0.5, 0.25), c = call("ccos", 0.5, 0.25), s2 = mul(s, s), c2 = mul(c, c);
    CHECK_NEAR(s2.re + c2.re, 1.0, 1e-13); CHECK_NEAR(s2.im + c2.im, 0.0, 1e-13); }

int main(void) { tc_begin(); TC_RUN(csin_zero); TC_RUN(csin_halfpi); TC_RUN(ccos_zero); TC_RUN(ccos_i); TC_RUN(csin_i); TC_RUN(csin_conj); TC_RUN(pythagorean); return tc_end(); }
