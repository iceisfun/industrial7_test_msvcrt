/* TC
 * id:    gcx_neg_cabs_overflow_result
 * area:  math_c99
 * funcs:
 * procs: cabs cabsf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.4.3 (hypot overflow); MS docs cabs
 * note:  errno on overflow deliberately not asserted here (see gcx_neg_probe_cabs_overflow_errno).
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

TC_CASE(exported) { CHECK(p_cabs && p_cabsf); }
TC_CASE(overflow_inf)
{
    if (!(p_cabs && p_cabsf)) TC_SKIP("not exported");
    double inf = INF_(), r;
    r = p_cabs(D(DBL_MAX,DBL_MAX)); CHECK_BITS_D(r, inf);
    r = p_cabs(D(-DBL_MAX,DBL_MAX)); CHECK_BITS_D(r, inf);
    CHECK_BITS_F(p_cabsf(F(FLT_MAX,FLT_MAX)), (float)inf);
}
TC_CASE(no_false_overflow)
{
    if (!(p_cabs && p_cabsf)) TC_SKIP("not exported");
    CHECK_BITS_D(p_cabs(D(DBL_MAX,0.0)), DBL_MAX);
    CHECK_BITS_D(p_cabs(D(0.0,-DBL_MAX)), DBL_MAX);
    CHECK_BITS_F(p_cabsf(F(FLT_MAX,0.0f)), FLT_MAX);
}

int main(void)
{
    tc_begin();
    p_cabs = (pt_cabs)tc_crt_proc("cabs");
    p_cabsf = (pt_cabsf)tc_crt_proc("cabsf");
    TC_RUN(exported); TC_RUN(overflow_inf); TC_RUN(no_false_overflow);
    return tc_end();
}
