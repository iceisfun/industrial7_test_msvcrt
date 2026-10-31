/* TC
 * id:    gcy_neg_csin_ccos_overflow
 * area:  math_c99
 * funcs:
 * procs: csinh ccosh csin ccos
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.3.5.4, 7.3.5.5, 7.3.6.4, 7.3.6.5 (results too large overflow to infinity); G.6
 * note:  Only components whose sign and infinity are unambiguous are asserted; zero-ish components are in the probe test.
 */
#include "tc.h"
#include <math.h>
typedef struct { double re, im; } cplx;   /* MS _Dcomplex: 16 bytes, hidden pointer on x64 */
typedef cplx (__cdecl *cfn_t)(cplx);
static volatile double v_zero = 0.0, v_one = 1.0;
static double PINF(void) { return v_one / v_zero; }
static double QNAN(void) { return PINF() - PINF(); }
static const double PI = 3.14159265358979323846;
static cplx C(double re, double im) { cplx z; z.re = re; z.im = im; return z; }
static cplx call(const char *name, double re, double im)
{
    cfn_t f = (cfn_t)tc_crt_proc(name);
    cplx z = C(re, im), r = C(0, 0);
    if (!f) { tc_fail_expr(__FILE__, __LINE__, "export missing"); return r; }
    return f(z);
}
static int isnan_(double d) { return d != d; }

TC_CASE(csinh_pos) { cplx r = call("csinh", 1000.0, 1.0); CHECK(r.re == PINF()); CHECK(r.im == PINF()); }
TC_CASE(csinh_neg) { cplx r = call("csinh", -1000.0, 1.0); CHECK(r.re == -PINF()); CHECK(r.im == PINF()); }  /* sinh(-x)<0, cosh(-x)*sin(1)>0 */
TC_CASE(ccosh_real) { cplx r = call("ccosh", 1000.0, 0.0); CHECK(r.re == PINF()); }
TC_CASE(csin_imag) { cplx r = call("csin", 0.0, 1000.0); CHECK(r.im == PINF()); }
TC_CASE(ccos_real) { cplx r = call("ccos", 0.0, 1000.0); CHECK(r.re == PINF()); }
TC_CASE(iph_untouched) { tc_iph_install(); tc_iph_count = 0;
    call("csinh", 1000.0, 1.0); call("ccosh", 1000.0, 0.0); call("csin", 0.0, 1000.0); call("ccos", 0.0, 1000.0);
    CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_RUN(csinh_pos); TC_RUN(csinh_neg); TC_RUN(ccosh_real); TC_RUN(csin_imag); TC_RUN(ccos_real); TC_RUN(iph_untouched); return tc_end(); }
