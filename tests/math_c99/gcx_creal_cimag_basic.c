/* TC
 * id:    gcx_creal_cimag_basic
 * area:  math_c99
 * funcs:
 * procs: creal cimag crealf cimagf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.3.9.3, 7.3.9.4; MS docs creal/cimag
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
typedef double (__cdecl *pt_creal)(Dc); static pt_creal p_creal;
typedef double (__cdecl *pt_cimag)(Dc); static pt_cimag p_cimag;
typedef float (__cdecl *pt_crealf)(Fc); static pt_crealf p_crealf;
typedef float (__cdecl *pt_cimagf)(Fc); static pt_cimagf p_cimagf;

TC_CASE(exported) { CHECK(p_creal && p_cimag && p_crealf && p_cimagf); }
TC_CASE(simple)
{
    if (!(p_creal && p_cimag)) TC_SKIP("not exported");
    CHECK_BITS_D(p_creal(D(3,4)), 3.0); CHECK_BITS_D(p_cimag(D(3,4)), 4.0);
    CHECK_BITS_D(p_creal(D(-3,-4)), -3.0); CHECK_BITS_D(p_cimag(D(-3,-4)), -4.0);
    CHECK_BITS_D(p_creal(D(DBL_MAX,-DBL_MAX)), DBL_MAX);
    CHECK_BITS_D(p_cimag(D(DBL_MAX,-DBL_MAX)), -DBL_MAX);
}
TC_CASE(signed_zero)
{
    if (!(p_creal && p_cimag)) TC_SKIP("not exported");
    CHECK_BITS_D(p_creal(D(-0.0,5.0)), -0.0);
    CHECK_BITS_D(p_cimag(D(5.0,-0.0)), -0.0);
    CHECK_BITS_D(p_creal(D(0.0,-0.0)), 0.0);
}
TC_CASE(inf_nan)
{
    if (!(p_creal && p_cimag)) TC_SKIP("not exported");
    double inf = INF_(), nan = NAN_();
    CHECK_BITS_D(p_creal(D(inf,nan)), inf);
    CHECK_IS_NAN(p_cimag(D(inf,nan)));
    CHECK_IS_NAN(p_creal(D(nan,1.0)));
}
TC_CASE(denormal)
{
    if (!(p_creal && p_cimag)) TC_SKIP("not exported");
    volatile double d = 4.9406564584124654e-324;
    CHECK_BITS_D(p_creal(D(d,-d)), d);
    CHECK_BITS_D(p_cimag(D(d,-d)), -d);
}
TC_CASE(floats)
{
    if (!(p_crealf && p_cimagf)) TC_SKIP("not exported");
    CHECK_BITS_F(p_crealf(F(1.5f,-2.5f)), 1.5f);
    CHECK_BITS_F(p_cimagf(F(1.5f,-2.5f)), -2.5f);
    CHECK_BITS_F(p_crealf(F(-0.0f,1.0f)), -0.0f);
}

int main(void)
{
    tc_begin();
    p_creal = (pt_creal)tc_crt_proc("creal");
    p_cimag = (pt_cimag)tc_crt_proc("cimag");
    p_crealf = (pt_crealf)tc_crt_proc("crealf");
    p_cimagf = (pt_cimagf)tc_crt_proc("cimagf");
    TC_RUN(exported); TC_RUN(simple); TC_RUN(signed_zero); TC_RUN(inf_nan); TC_RUN(denormal); TC_RUN(floats);
    return tc_end();
}
