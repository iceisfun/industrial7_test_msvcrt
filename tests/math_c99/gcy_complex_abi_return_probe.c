/* TC
 * id:    gcy_complex_abi_return_probe
 * area:  math_c99
 * funcs:
 * procs: cexp csin
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _Dcomplex; MS x64 ABI (16-byte aggregates by hidden pointer)
 * note:  PROBING that a struct {double re,im} typedef passed/returned through a function pointer matches the CRT ABI:
 *        cexp(0+0i)==1+0i and csin(0+0i)==0+0i. If this fails, other complex tests' failures are ABI artefacts.
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

TC_CASE(cexp_identity) { cplx r = call("cexp", 0.0, 0.0); CHECK_BITS_D(r.re, 1.0); CHECK_BITS_D(r.im, 0.0); }
TC_CASE(csin_zero) { cplx r = call("csin", 0.0, 0.0); CHECK_BITS_D(r.re, 0.0); CHECK_BITS_D(r.im, 0.0); }
int main(void) { tc_begin(); TC_RUN(cexp_identity); TC_RUN(csin_zero); return tc_end(); }
