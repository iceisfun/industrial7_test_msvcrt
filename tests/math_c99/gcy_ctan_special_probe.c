/* TC
 * id:    gcy_ctan_special_probe
 * area:  math_c99
 * funcs: 
 * procs: ctan
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 G.6.2.6
 * note:  PROBE: ctan(1+i*inf)=0+i1 and ctan(inf+i)=NaN+iNaN per Annex G.
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

TC_CASE(y_inf) { cplx a = call("ctan", 1.0, PINF()); CHECK(isz(a.re)); CHECK_BITS_D(a.im, 1.0); }
TC_CASE(x_inf) { cplx a = call("ctan", PINF(), 1.0); CHECK_IS_NAN(a.re); CHECK_IS_NAN(a.im); }

int main(void) { tc_begin(); TC_RUN(y_inf); TC_RUN(x_inf); return tc_end(); }
