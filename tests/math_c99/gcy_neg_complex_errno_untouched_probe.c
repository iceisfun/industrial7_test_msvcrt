/* TC
 * id:    gcy_neg_complex_errno_untouched_probe
 * area:  math_c99
 * funcs:
 * procs: cexp clog catanh ctan _errno
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.3.2 / Annex G (errno not required for complex functions)
 * note:  PROBING: errno is left at a sentinel and the invalid-parameter handler is not called by clog(0), catanh(1),
 *        cexp(1000), ctan(inf+1i). Real Windows might set ERANGE/EDOM through underlying real-math calls.
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

typedef int *(__cdecl *errno_t_fn)(void);
static int run(const char *name, double re, double im)
{
    errno_t_fn pe = (errno_t_fn)tc_crt_proc("_errno");
    int e;
    if (!pe) { tc_fail_expr(__FILE__, __LINE__, "_errno missing"); return 0; }
    tc_iph_install(); tc_iph_count = 0;
    *pe() = 0x5a5a;
    call(name, re, im);
    e = *pe();
    CHECK_EQ_I(tc_iph_count, 0);
    return e;
}
TC_CASE(clog_zero) { CHECK_EQ_I(run("clog", 0.0, 0.0), 0x5a5a); }
TC_CASE(catanh_one) { CHECK_EQ_I(run("catanh", 1.0, 0.0), 0x5a5a); }
TC_CASE(cexp_1000) { CHECK_EQ_I(run("cexp", 1000.0, 0.0), 0x5a5a); }
TC_CASE(ctan_inf) { CHECK_EQ_I(run("ctan", PINF(), 1.0), 0x5a5a); }

int main(void) { tc_begin(); TC_RUN(clog_zero); TC_RUN(catanh_one); TC_RUN(cexp_1000); TC_RUN(ctan_inf); return tc_end(); }
