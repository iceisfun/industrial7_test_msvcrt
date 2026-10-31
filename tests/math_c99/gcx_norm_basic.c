/* TC
 * id:    gcx_norm_basic
 * area:  math_c99
 * funcs:
 * procs: norm normf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   MS docs norm (complex.h; not in C11); norm(z)=re^2+im^2
 * note:  Only exactly-representable results are asserted bit-exact (3-4-5, 2-0, 1.5-2 -> 2.5^2); others use a tolerance since the rounding path (re*re+im*im vs cabs^2) is unspecified.
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
TC_CASE(double)
{
    if (!(p_norm)) TC_SKIP("not exported");
    CHECK_BITS_D(p_norm(D(3,4)), 25.0);
    CHECK_BITS_D(p_norm(D(-3,-4)), 25.0);
    CHECK_BITS_D(p_norm(D(0.0,0.0)), 0.0);
    CHECK_BITS_D(p_norm(D(-0.0,-0.0)), 0.0);
    CHECK_BITS_D(p_norm(D(2,0)), 4.0);
    CHECK_BITS_D(p_norm(D(0,3)), 9.0);
    CHECK_BITS_D(p_norm(D(1.5,2)), 6.25);
}
TC_CASE(float)
{
    if (!(p_normf)) TC_SKIP("not exported");
    CHECK_BITS_F(p_normf(F(3.0f,4.0f)), 25.0f);
    CHECK_NEAR(p_normf(F(0.5f,0.5f)), 0.5, 1e-6); /* tolerant: an impl via cabs()^2 is not exact */
}
TC_CASE(sum_of_squares)
{
    if (!(p_norm)) TC_SKIP("not exported");
    static const double xs[] = { 0.5, 1.25, -2.0, 7.0, -0.125 }; size_t i, j;
    for (i = 0; i < 5; i++) for (j = 0; j < 5; j++) {
        volatile double a = xs[i], b = xs[j];
        CHECK_NEAR(p_norm(D(a,b)), a*a + b*b, 1e-12); /* tolerant: formula, not rounding path */
    }
}

int main(void)
{
    tc_begin();
    p_norm = (pt_norm)tc_crt_proc("norm");
    p_normf = (pt_normf)tc_crt_proc("normf");
    TC_RUN(exported); TC_RUN(double); TC_RUN(float); TC_RUN(sum_of_squares);
    return tc_end();
}
