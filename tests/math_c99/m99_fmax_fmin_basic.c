/* TC
 * id:    m99_fmax_fmin_basic
 * area:  math_c99
 * funcs: fmax fmin fmaxf fminf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.12.2, 7.12.12.3; MS docs fmax, fmin
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
    volatile double a = 1, b = 2, na = -1, nb = -2;
    CHECK_BITS_D(fmax(a, b), 2.0); CHECK_BITS_D(fmax(b, a), 2.0);
    CHECK_BITS_D(fmin(a, b), 1.0); CHECK_BITS_D(fmin(b, a), 1.0);
    CHECK_BITS_D(fmax(na, nb), -1.0); CHECK_BITS_D(fmin(na, nb), -2.0);
    CHECK_BITS_D(fmax(na, b), 2.0); CHECK_BITS_D(fmin(na, b), -1.0);
}
TC_CASE(infinities)
{
    volatile double x = 42.5, i = d_inf();
    CHECK_BITS_D(fmax(-i, x), 42.5); CHECK_BITS_D(fmax(x, -i), 42.5);
    CHECK_BITS_D(fmin(i, x), 42.5); CHECK_BITS_D(fmin(x, i), 42.5);
    CHECK_BITS_D(fmax(i, x), d_inf()); CHECK_BITS_D(fmin(-i, x), -d_inf());
}
TC_CASE(floats)
{
    volatile float a = 1.5f, b = -2.5f, i = f_inf();
    CHECK_BITS_F(fmaxf(a, b), 1.5f); CHECK_BITS_F(fminf(a, b), -2.5f);
    CHECK_BITS_F(fmaxf(b, a), 1.5f); CHECK_BITS_F(fminf(b, a), -2.5f);
    CHECK_BITS_F(fmaxf(-i, a), 1.5f); CHECK_BITS_F(fminf(i, a), 1.5f);
}
TC_CASE(equal_args)
{
    volatile double a = 3.25; volatile float f = -7.5f;
    CHECK_BITS_D(fmax(a, a), 3.25); CHECK_BITS_D(fmin(a, a), 3.25);
    CHECK_BITS_F(fmaxf(f, f), -7.5f); CHECK_BITS_F(fminf(f, f), -7.5f);
}
int main(void) { tc_begin(); TC_RUN(ordinary); TC_RUN(infinities); TC_RUN(floats); TC_RUN(equal_args); return tc_end(); }
