/* TC
 * id:    gcx_carg_special
 * area:  math_c99
 * funcs:
 * procs: carg
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.3.9.1, F.10.1.4 (atan2)
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
typedef double (__cdecl *pt_carg)(Dc); static pt_carg p_carg;

TC_CASE(exported) { CHECK(p_carg); }
TC_CASE(infinities)
{
    if (!(p_carg)) TC_SKIP("not exported");
    const double pi = 3.14159265358979323846;
    double inf = INF_();
    CHECK_NEAR(p_carg(D(-inf,0.0)), pi, 1e-15);
    CHECK_BITS_D(p_carg(D(inf,0.0)), 0.0);
    CHECK_NEAR(p_carg(D(inf,inf)), pi/4, 1e-15);
    CHECK_NEAR(p_carg(D(-inf,inf)), 3*pi/4, 1e-15);
    CHECK_NEAR(p_carg(D(inf,-inf)), -pi/4, 1e-15);
    CHECK_NEAR(p_carg(D(0.0,inf)), pi/2, 1e-15);
}
TC_CASE(nan)
{
    if (!(p_carg)) TC_SKIP("not exported");
    double nan = NAN_();
    CHECK_IS_NAN(p_carg(D(nan,1.0)));
    CHECK_IS_NAN(p_carg(D(1.0,nan)));
}

int main(void)
{
    tc_begin();
    p_carg = (pt_carg)tc_crt_proc("carg");
    TC_RUN(exported); TC_RUN(infinities); TC_RUN(nan);
    return tc_end();
}
