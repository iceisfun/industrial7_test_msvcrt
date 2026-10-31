/* TC
 * id:    m99_remainder_neg_domain
 * area:  math_c99
 * funcs: remainder remainderf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.7.2; MS docs remainder
 * note:  errno accepted as 0 or EDOM; only the NaN result is asserted exactly.
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

TC_CASE(div_by_zero) { volatile double x = 1.0, z = 0.0; errno = 0; CHECK_IS_NAN(remainder(x, z)); CHECK(errno == 0 || errno == EDOM); 
    errno = 0; CHECK_IS_NAN(remainder(x, -z)); CHECK(errno == 0 || errno == EDOM); }
TC_CASE(zero_by_zero) { volatile double z = 0.0; errno = 0; CHECK_IS_NAN(remainder(z, z)); CHECK(errno == 0 || errno == EDOM); }
TC_CASE(inf_dividend) { volatile double t = 2.0; double i = d_inf(); errno = 0; CHECK_IS_NAN(remainder(i, t)); CHECK(errno == 0 || errno == EDOM);
    errno = 0; CHECK_IS_NAN(remainder(-i, t)); CHECK(errno == 0 || errno == EDOM); }
TC_CASE(inf_inf) { double i = d_inf(); errno = 0; CHECK_IS_NAN(remainder(i, i)); CHECK(errno == 0 || errno == EDOM); }
TC_CASE(float_div_by_zero) { volatile float x = 1.0f, z = 0.0f; float r; errno = 0; r = remainderf(x, z); CHECK(r != r); CHECK(errno == 0 || errno == EDOM); }
TC_CASE(float_inf_dividend) { volatile float t = 2.0f; float i = f_inf(), r; errno = 0; r = remainderf(i, t); CHECK(r != r); CHECK(errno == 0 || errno == EDOM); }
int main(void) { tc_begin(); TC_RUN(div_by_zero); TC_RUN(zero_by_zero); TC_RUN(inf_dividend); TC_RUN(inf_inf); TC_RUN(float_div_by_zero); TC_RUN(float_inf_dividend); return tc_end(); }
