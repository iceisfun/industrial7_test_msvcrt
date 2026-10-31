/* TC
 * id:    gcx_cabs_basic
 * area:  math_c99
 * funcs:
 * procs: cabs cabsf cabsl
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 G.6, 7.3.8.1; MS docs cabs
 * note:  Hand-written struct typedefs: MSVC _Dcomplex/_Fcomplex are structs (x64: 16-byte struct by reference, 8-byte _Fcomplex in a register). cabsl takes {double,double} since MS long double == double.
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
typedef double (__cdecl *pt_cabsl)(Dc); static pt_cabsl p_cabsl;

TC_CASE(exported) { CHECK(p_cabs && p_cabsf && p_cabsl); }
TC_CASE(double_vals)
{
    if (!(p_cabs)) TC_SKIP("not exported");
    CHECK_BITS_D(p_cabs(D(3,4)), 5.0);
    CHECK_BITS_D(p_cabs(D(-3,-4)), 5.0);
    CHECK_BITS_D(p_cabs(D(0,-7)), 7.0);
}
TC_CASE(zeros)
{
    if (!(p_cabs)) TC_SKIP("not exported");
    CHECK_BITS_D(p_cabs(D(0.0,0.0)), 0.0);
    CHECK_BITS_D(p_cabs(D(-0.0,-0.0)), 0.0);
}
TC_CASE(real_only)
{
    if (!(p_cabs)) TC_SKIP("not exported");
    CHECK_BITS_D(p_cabs(D(2.5,0.0)), 2.5);
    CHECK_BITS_D(p_cabs(D(-2.5,0.0)), 2.5);
}
TC_CASE(float)
{
    if (!(p_cabsf)) TC_SKIP("not exported");
    CHECK_BITS_F(p_cabsf(F(3.0f,4.0f)), 5.0f);
    CHECK_BITS_F(p_cabsf(F(-3.0f,-4.0f)), 5.0f);
}
TC_CASE(long_double)
{
    if (!(p_cabsl)) TC_SKIP("not exported");
    /* MS long double is 64-bit: _Ldcomplex is {double,double} */
    CHECK_BITS_D(p_cabsl(D(3,4)), 5.0);
    CHECK_BITS_D(p_cabsl(D(0,-7)), 7.0);
}

int main(void)
{
    tc_begin();
    p_cabs = (pt_cabs)tc_crt_proc("cabs");
    p_cabsf = (pt_cabsf)tc_crt_proc("cabsf");
    p_cabsl = (pt_cabsl)tc_crt_proc("cabsl");
    TC_RUN(exported); TC_RUN(double_vals); TC_RUN(zeros); TC_RUN(real_only); TC_RUN(float); TC_RUN(long_double);
    return tc_end();
}
