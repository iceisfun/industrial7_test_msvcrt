/* TC
 * id:    gcy_cplx_signed_zero_probe
 * area:  math_c99
 * funcs: 
 * procs: cexp clog csin ctan csinh cacos casinh
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 G.6, 7.3.3 branch cuts
 * note:  PROBE: signed-zero results of MS complex functions per Annex G (not guaranteed since MS does not define __STDC_IEC_559_COMPLEX__).
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

TC_CASE(cexp_conj) { cplx a = call("cexp", 2.0, 0.0), b = call("cexp", 2.0, v_nzero); CHECK_BITS_D(a.im, 0.0); CHECK_BITS_D(b.im, -0.0); CHECK_BITS_D(a.re, b.re); }
TC_CASE(clog_cut_below) { cplx r = call("clog", -1.0, v_nzero); CHECK_BITS_D(r.re, 0.0); CHECK_NEAR(r.im, -PI, 1e-15); }
TC_CASE(csin_negzero) { cplx b = call("csin", v_nzero, 0.0); CHECK_BITS_D(b.re, -0.0); CHECK_BITS_D(b.im, 0.0); }
TC_CASE(ctan_negzero) { cplx b = call("ctan", v_nzero, 0.0); CHECK_BITS_D(b.re, -0.0); CHECK_BITS_D(b.im, 0.0); }
TC_CASE(csinh_negzero) { cplx b = call("csinh", v_nzero, 0.0); CHECK_BITS_D(b.re, -0.0); CHECK_BITS_D(b.im, 0.0); }
TC_CASE(cacos_zero) { cplx b = call("cacos", 0.0, 0.0); CHECK_BITS_D(b.im, -0.0); }
TC_CASE(casinh_negzero) { cplx b = call("casinh", v_nzero, 0.0); CHECK_BITS_D(b.re, -0.0); CHECK_BITS_D(b.im, 0.0); }

int main(void) { tc_begin(); TC_RUN(cexp_conj); TC_RUN(clog_cut_below); TC_RUN(csin_negzero); TC_RUN(ctan_negzero); TC_RUN(csinh_negzero); TC_RUN(cacos_zero); TC_RUN(casinh_negzero); return tc_end(); }
