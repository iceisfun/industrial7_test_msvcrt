/* TC
 * id:    m99_erf_erfc_basic
 * area:  math_c99
 * funcs: erf erff erfc erfcf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.8.1 7.12.8.2; reference values from standard mathematical tables
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define PINF (v_one / v_zero)
#define QNAN (PINF - PINF)
#define FPINF (f_one / f_zero)
#define FQNAN (FPINF - FPINF)

TC_CASE(zero) { volatile double z = 0.0; CHECK_BITS_D(erf(z), 0.0); CHECK_BITS_D(erfc(z), 1.0); }
TC_CASE(one) { volatile double x = 1.0; CHECK_NEAR(erf(x), 0.8427007929497149, 1e-15); CHECK_NEAR(erfc(x), 0.1572992070502851, 1e-15); }
TC_CASE(half) { volatile double x = 0.5; CHECK_NEAR(erf(x), 0.5204998778130465, 1e-15); }
TC_CASE(odd_and_complement) { volatile double x = 0.75, y = -0.75; CHECK_NEAR(erf(y), -erf(x), 1e-15); CHECK(erf(y) < 0.0); CHECK_NEAR(erf(x) + erfc(x), 1.0, 1e-15); }
TC_CASE(float_one) { volatile float x = 1.0f; CHECK_NEAR(erff(x), 0.84270079f, 1e-6); CHECK_NEAR(erfcf(x), 0.15729921f, 1e-6); }

int main(void) { tc_begin(); TC_RUN(zero); TC_RUN(one); TC_RUN(half); TC_RUN(odd_and_complement); TC_RUN(float_one); return tc_end(); }
