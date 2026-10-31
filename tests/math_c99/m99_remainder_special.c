/* TC
 * id:    m99_remainder_special
 * area:  math_c99
 * funcs: remainder remainderf
 * crt:   ucrtbase msvcr120
 * tier:  p0

 * conf:  standard
 * ref:   C11 F.10.7.2; MS docs remainder, remainderf
 * note:  remainder(+-0,y)=+-0 and remainder(x,+-inf)=x (F.10.7.2). The sign of an exactly-zero NONZERO-dividend result is probed separately in m99_remainder_zero_sign_probe.
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

TC_CASE(inf_divisor) { volatile double x = 3.5, n = -7.25; double i = d_inf();
    CHECK_BITS_D(remainder(x, i), 3.5); CHECK_BITS_D(remainder(n, i), -7.25); CHECK_BITS_D(remainder(x, -i), 3.5); }
TC_CASE(float_inf_divisor) { volatile float x = 3.5f; float i = f_inf(); CHECK_BITS_F(remainderf(x, i), 3.5f); CHECK_BITS_F(remainderf(x, -i), 3.5f); }
TC_CASE(zero_dividend) { volatile double pz = 0.0, nz = -0.0, y = 5.0, ny = -5.0;
    CHECK_BITS_D(remainder(pz, y), 0.0); CHECK_BITS_D(remainder(nz, y), -0.0);
    CHECK_BITS_D(remainder(nz, ny), -0.0); CHECK_BITS_D(remainder(pz, ny), 0.0); }
TC_CASE(rounding_nearest_even) { volatile double a = 5.0, b = 3.0, c = 7.0, d = 2.0, e = 5.0, f = 2.0;
    CHECK_BITS_D(remainder(a, b), -1.0); CHECK_BITS_D(remainder(c, d), -1.0); CHECK_BITS_D(remainder(e, f), 1.0); }
TC_CASE(nan_passthrough) { volatile double x = 2.0; double n = d_nan(); volatile float fx = 2.0f; float fn = f_nan();
    CHECK_IS_NAN(remainder(n, x)); CHECK_IS_NAN(remainder(x, n)); { float r = remainderf(fn, fx); CHECK(r != r); } { float r = remainderf(fx, fn); CHECK(r != r); } }
int main(void) { tc_begin(); TC_RUN(inf_divisor); TC_RUN(float_inf_divisor); TC_RUN(zero_dividend); TC_RUN(rounding_nearest_even); TC_RUN(nan_passthrough); return tc_end(); }
