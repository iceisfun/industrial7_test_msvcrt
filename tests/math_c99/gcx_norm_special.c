/* TC
 * id:    gcx_norm_special
 * area:  math_c99
 * funcs:
 * procs: norm normf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs norm; IEEE 754 arithmetic
 * note:  inferred from IEEE arithmetic if implemented as re*re+im*im. inf+nan mixed is not asserted.
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
typedef double (__cdecl *pt_norm)(Dc); static pt_norm p_norm;
typedef float (__cdecl *pt_normf)(Fc); static pt_normf p_normf;

TC_CASE(exported) { CHECK(p_norm && p_normf); }
TC_CASE(inf_nan)
{
    if (!(p_norm)) TC_SKIP("not exported");
    double inf = INF_(), nan = NAN_();
    CHECK_BITS_D(p_norm(D(inf,0.0)), inf);
    CHECK_BITS_D(p_norm(D(0.0,-inf)), inf);
    CHECK_IS_NAN(p_norm(D(nan,1.0)));
    CHECK_IS_NAN(p_norm(D(1.0,nan)));
}
TC_CASE(overflow)
{
    if (!(p_norm && p_normf)) TC_SKIP("not exported");
    CHECK_BITS_D(p_norm(D(1e200,1e200)), INF_());
    CHECK_BITS_F(p_normf(F(1e30f,1e30f)), (float)INF_());
}
TC_CASE(underflow)
{
    if (!(p_norm)) TC_SKIP("not exported");
    double r = p_norm(D(1e-200,1e-200));
    CHECK(r < DBL_MIN); CHECK(r >= 0.0);
}

int main(void)
{
    tc_begin();
    p_norm = (pt_norm)tc_crt_proc("norm");
    p_normf = (pt_normf)tc_crt_proc("normf");
    TC_RUN(exported); TC_RUN(inf_nan); TC_RUN(overflow); TC_RUN(underflow);
    return tc_end();
}
