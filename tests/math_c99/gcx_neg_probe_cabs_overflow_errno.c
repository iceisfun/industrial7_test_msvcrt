/* TC
 * id:    gcx_neg_probe_cabs_overflow_errno
 * area:  math_c99
 * funcs:
 * procs: cabs cabsf norm
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs cabs/_hypot (errno ERANGE on overflow)
 * note:  PROBE: whether real Windows cabs/cabsf set errno=ERANGE on overflow (likely yes, hypot-style) and norm (plain arithmetic) leaves errno 0.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

typedef struct { double re, im; } Dc;
typedef struct { float re, im; } Fc;
static Dc D(double r, double i) { Dc z; z.re = r; z.im = i; return z; }
static Fc F(float r, float i) { Fc z; z.re = r; z.im = i; return z; }
static volatile double v_zero = 0.0, v_one = 1.0;
static double INF_(void) { return v_one / v_zero; }
static double NAN_(void) { return INF_() - INF_(); }
typedef double (__cdecl *pt_cabs)(Dc); static pt_cabs p_cabs;
typedef float (__cdecl *pt_cabsf)(Fc); static pt_cabsf p_cabsf;
typedef double (__cdecl *pt_norm)(Dc); static pt_norm p_norm;

TC_CASE(exported) { CHECK(p_cabs && p_cabsf && p_norm); }
TC_CASE(cabs_erange)
{
    if (!(p_cabs)) TC_SKIP("not exported");
    errno = 0; p_cabs(D(DBL_MAX,DBL_MAX));
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(cabsf_erange)
{
    if (!(p_cabsf)) TC_SKIP("not exported");
    errno = 0; p_cabsf(F(FLT_MAX,FLT_MAX));
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(norm_no_errno)
{
    if (!(p_norm)) TC_SKIP("not exported");
    errno = 0; p_norm(D(1e200,1e200));
    CHECK_EQ_I(errno, 0);
}

int main(void)
{
    tc_begin();
    p_cabs = (pt_cabs)tc_crt_proc("cabs");
    p_cabsf = (pt_cabsf)tc_crt_proc("cabsf");
    p_norm = (pt_norm)tc_crt_proc("norm");
    TC_RUN(exported); TC_RUN(cabs_erange); TC_RUN(cabsf_erange); TC_RUN(norm_no_errno);
    return tc_end();
}
