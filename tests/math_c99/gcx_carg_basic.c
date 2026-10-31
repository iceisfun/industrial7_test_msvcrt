/* TC
 * id:    gcx_carg_basic
 * area:  math_c99
 * funcs:
 * procs: carg cargf cargl
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.3.9.1; MS docs carg
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
typedef double (__cdecl *pt_cargl)(Dc); static pt_cargl p_cargl;

TC_CASE(exported) { CHECK(p_carg && p_cargf && p_cargl); }
TC_CASE(axes)
{
    if (!(p_carg)) TC_SKIP("not exported");
    const double pi = 3.14159265358979323846;
    CHECK_BITS_D(p_carg(D(1,0)), 0.0);
    CHECK_NEAR(p_carg(D(0,1)), pi/2, 1e-15);
    CHECK_NEAR(p_carg(D(-1,0)), pi, 1e-15);
    CHECK_NEAR(p_carg(D(0,-1)), -pi/2, 1e-15);
}
TC_CASE(diagonals)
{
    if (!(p_carg)) TC_SKIP("not exported");
    const double pi = 3.14159265358979323846;
    CHECK_NEAR(p_carg(D(1,1)), pi/4, 1e-15);
    CHECK_NEAR(p_carg(D(-1,-1)), -3*pi/4, 1e-15);
}
TC_CASE(float_long)
{
    if (!(p_cargf && p_cargl)) TC_SKIP("not exported");
    const double pi = 3.14159265358979323846;
    CHECK_NEAR(p_cargf(F(0.0f,1.0f)), pi/2, 1e-6);
    CHECK_NEAR(p_cargl(D(-1,0)), pi, 1e-15);
}

int main(void)
{
    tc_begin();
    p_carg = (pt_carg)tc_crt_proc("carg");
    p_cargf = (pt_cargf)tc_crt_proc("cargf");
    p_cargl = (pt_cargl)tc_crt_proc("cargl");
    TC_RUN(exported); TC_RUN(axes); TC_RUN(diagonals); TC_RUN(float_long);
    return tc_end();
}
