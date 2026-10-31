/* TC
 * id:    gcx_carg_signed_zero
 * area:  math_c99
 * funcs:
 * procs: carg cargf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.3.9.1, G.6, F.10.1.4 (atan2); MS docs carg
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
typedef float (__cdecl *pt_cargf)(Fc); static pt_cargf p_cargf;

TC_CASE(exported) { CHECK(p_carg && p_cargf); }
TC_CASE(branch_cut)
{
    if (!(p_carg)) TC_SKIP("not exported");
    const double pi = 3.14159265358979323846;
    double a = p_carg(D(-1.0,0.0)), b = p_carg(D(-1.0,-0.0));
    CHECK_NEAR(a, pi, 1e-15); CHECK(a > 0);
    CHECK_NEAR(b, -pi, 1e-15); CHECK(b < 0);
}
TC_CASE(zero_zero)
{
    if (!(p_carg)) TC_SKIP("not exported");
    const double pi = 3.14159265358979323846;
    CHECK_BITS_D(p_carg(D(0.0,0.0)), 0.0);
    CHECK_BITS_D(p_carg(D(0.0,-0.0)), -0.0);
    CHECK_NEAR(p_carg(D(-0.0,0.0)), pi, 1e-15);
    CHECK_NEAR(p_carg(D(-0.0,-0.0)), -pi, 1e-15);
}
TC_CASE(float)
{
    if (!(p_cargf)) TC_SKIP("not exported");
    float r = p_cargf(F(-1.0f,-0.0f));
    CHECK(r < 0); CHECK_NEAR(r, -3.14159265358979, 1e-6);
}

int main(void)
{
    tc_begin();
    p_carg = (pt_carg)tc_crt_proc("carg");
    p_cargf = (pt_cargf)tc_crt_proc("cargf");
    TC_RUN(exported); TC_RUN(branch_cut); TC_RUN(zero_zero); TC_RUN(float);
    return tc_end();
}
