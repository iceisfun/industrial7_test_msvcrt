/* TC
 * id:    gcy_neg_csin_ccos_overflow_zero_probe
 * area:  math_c99
 * funcs:
 * procs: csin ccos ctanh
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 G.6.2 (finite-argument overflow combined with zero parts is not specified)
 * note:  PROBING: when one part overflows, whether the other part stays exactly 0 (not NaN from 0*inf) for csin(0+1000i),
 *        and whether ctanh(1000+0i) saturates to real exactly 1 (not inf/inf = NaN).
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

TC_CASE(csin_real_zero) { cplx r = call("csin", 0.0, 1000.0); CHECK(r.re == 0.0); }
TC_CASE(ccos_imag_zero) { cplx r = call("ccos", 0.0, 1000.0); CHECK(r.im == 0.0); }
TC_CASE(ctanh_saturates) { cplx r = call("ctanh", 1000.0, 0.0); CHECK_BITS_D(r.re, 1.0); CHECK(r.im == 0.0); }

int main(void) { tc_begin(); TC_RUN(csin_real_zero); TC_RUN(ccos_imag_zero); TC_RUN(ctanh_saturates); return tc_end(); }
