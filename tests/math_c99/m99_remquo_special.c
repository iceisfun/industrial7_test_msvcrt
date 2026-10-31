/* TC
 * id:    m99_remquo_special
 * area:  math_c99
 * funcs: remquo remquof
 * crt:   ucrtbase msvcr120
 * tier:  p0

 * conf:  standard
 * ref:   C11 7.12.10.3, F.10.7.3; MS docs remquo
 * note:  Only the returned remainder is asserted. *quo for x/inf and 0/y (should be 0) is probed in m99_remquo_quo_zero_probe; NaN quo not asserted.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }

TC_CASE(finite_by_inf) { volatile double x = 1.0; double i = d_inf(); int q = 99; CHECK_BITS_D(remquo(x, i, &q), 1.0);
    CHECK_BITS_D(remquo(x, -i, &q), 1.0); }
TC_CASE(float_finite_by_inf) { volatile float x = 1.0f; float i = f_inf(); int q = 99; CHECK_BITS_F(remquof(x, i, &q), 1.0f); }
TC_CASE(zero_dividend) { volatile double pz = 0.0, nz = -0.0, y = 5.0; int q = 99; CHECK_BITS_D(remquo(pz, y, &q), 0.0);
    CHECK_BITS_D(remquo(nz, y, &q), -0.0); }
TC_CASE(nan_passthrough) { volatile double x = 2.0; double n = d_nan(); int q = 0; CHECK_IS_NAN(remquo(n, x, &q)); CHECK_IS_NAN(remquo(x, n, &q)); }
int main(void) { tc_begin(); TC_RUN(finite_by_inf); TC_RUN(float_finite_by_inf); TC_RUN(zero_dividend); TC_RUN(nan_passthrough); return tc_end(); }
