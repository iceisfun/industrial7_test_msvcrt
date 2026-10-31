/* TC
 * id:    gcx_cabs_special
 * area:  math_c99
 * funcs:
 * procs: cabs cabsf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 G.6, F.10.4.3 (hypot); MS docs cabs
 * note:  cabs(+-inf, NaN) split into gcx_probe_cabs_inf_nan (hypot-style special case, unproven for old msvcr120).
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
TC_CASE(inf_finite)
{
    if (!(p_cabs)) TC_SKIP("not exported");
    double inf = INF_();
    CHECK_BITS_D(p_cabs(D(inf,1.0)), inf);
    CHECK_BITS_D(p_cabs(D(-inf,-2.0)), inf);
    CHECK_BITS_D(p_cabs(D(3.0,-inf)), inf);
}
TC_CASE(nan)
{
    if (!(p_cabs)) TC_SKIP("not exported");
    double nan = NAN_();
    CHECK_IS_NAN(p_cabs(D(nan,1.0)));
    CHECK_IS_NAN(p_cabs(D(1.0,nan)));
    CHECK_IS_NAN(p_cabs(D(nan,nan)));
}
TC_CASE(tiny_and_big)
{
    if (!(p_cabs)) TC_SKIP("not exported");
    volatile double dm = 4.9406564584124654e-324;
    double r = p_cabs(D(dm,0.0));
    CHECK_BITS_D(r, dm);
    r = p_cabs(D(1e200,1e200));
    CHECK_NEAR(r/1e200, 1.4142135623730951, 1.4142135623730951e-14);
}

int main(void)
{
    tc_begin();
    p_cabs = (pt_cabs)tc_crt_proc("cabs");
    p_cabsf = (pt_cabsf)tc_crt_proc("cabsf");
    TC_RUN(exported); TC_RUN(inf_finite); TC_RUN(nan); TC_RUN(tiny_and_big);
    return tc_end();
}
