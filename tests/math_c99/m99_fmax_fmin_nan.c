/* TC
 * id:    m99_fmax_fmin_nan
 * area:  math_c99
 * funcs: fmax fmin fmaxf fminf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.9.2 (NaN treated as missing data); MS docs fmax, fmin
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

TC_CASE(one_nan_double)
{
    volatile double n = d_nan(), one = 1.0, ni = -d_inf();
    CHECK_BITS_D(fmax(n, one), 1.0); CHECK_BITS_D(fmax(one, n), 1.0);
    CHECK_BITS_D(fmin(n, one), 1.0); CHECK_BITS_D(fmin(one, n), 1.0);
    CHECK_BITS_D(fmax(n, ni), -d_inf()); CHECK_BITS_D(fmin(ni, n), -d_inf());
}
TC_CASE(both_nan)
{
    volatile double n = d_nan(); volatile float nf = f_nan();
    CHECK_IS_NAN(fmax(n, n)); CHECK_IS_NAN(fmin(n, n));
    CHECK(fmaxf(nf, nf) != fmaxf(nf, nf)); CHECK(fminf(nf, nf) != fminf(nf, nf));
}
TC_CASE(one_nan_float)
{
    volatile float n = f_nan(), one = 1.0f;
    CHECK_BITS_F(fmaxf(n, one), 1.0f); CHECK_BITS_F(fmaxf(one, n), 1.0f);
    CHECK_BITS_F(fminf(n, one), 1.0f); CHECK_BITS_F(fminf(one, n), 1.0f);
}
int main(void) { tc_begin(); TC_RUN(one_nan_double); TC_RUN(both_nan); TC_RUN(one_nan_float); return tc_end(); }
