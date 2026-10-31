/* TC
 * id:    m99_fdim_basic
 * area:  math_c99
 * funcs: fdim fdimf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.12.1, F.10.9.1; MS docs fdim
 */
#include "tc.h"
#include <math.h>
#include <float.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }

TC_CASE(ordinary)
{
    volatile double a = 5, b = 3, c = -1, d = -3;
    CHECK_BITS_D(fdim(a, b), 2.0); CHECK_BITS_D(fdim(b, a), 0.0); CHECK_BITS_D(fdim(c, d), 2.0);
    CHECK_BITS_D(fdim(a, a), 0.0);
}
TC_CASE(infinities)
{
    volatile double i = d_inf(), one = 1.0;
    CHECK_BITS_D(fdim(i, one), d_inf()); CHECK_BITS_D(fdim(one, i), 0.0);
}
TC_CASE(nans)
{
    volatile double n = d_nan(), one = 1.0;
    CHECK_IS_NAN(fdim(n, one)); CHECK_IS_NAN(fdim(one, n));
}
TC_CASE(floats)
{
    volatile float a = 5.0f, b = 3.0f;
    CHECK_BITS_F(fdimf(a, b), 2.0f); CHECK_BITS_F(fdimf(b, a), 0.0f);
    { float r = fdimf(f_nan(), a); CHECK(r != r); }
}
int main(void) { tc_begin(); TC_RUN(ordinary); TC_RUN(infinities); TC_RUN(nans); TC_RUN(floats); return tc_end(); }
