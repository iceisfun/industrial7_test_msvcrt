/* TC
 * id:    gcy_clog_basic
 * area:  math_c99
 * funcs: 
 * procs: clog
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.3.7.2, G.6.3.2
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

TC_CASE(one) { cplx r = call("clog", 1.0, 0.0); CHECK_BITS_D(r.re, 0.0); CHECK_BITS_D(r.im, 0.0); }
TC_CASE(e) { cplx r = call("clog", 2.718281828459045, 0.0); CHECK_NEAR(r.re, 1.0, 1e-15); CHECK_BITS_D(r.im, 0.0); }
TC_CASE(i) { cplx r = call("clog", 0.0, 1.0); CHECK_NEAR(r.re, 0.0, 1e-15); CHECK_NEAR(r.im, PI / 2, 1e-15); }
TC_CASE(minus_one) { cplx r = call("clog", -1.0, 0.0); CHECK_BITS_D(r.re, 0.0); CHECK_NEAR(r.im, PI, 1e-15); }
TC_CASE(three_four) { cplx r = call("clog", 3.0, 4.0); CHECK_NEAR(r.re, 1.6094379124341003, 1e-14); CHECK_NEAR(r.im, 0.9272952180016122, 1e-14); }

int main(void) { tc_begin(); TC_RUN(one); TC_RUN(e); TC_RUN(i); TC_RUN(minus_one); TC_RUN(three_four); return tc_end(); }
