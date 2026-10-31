/* TC
 * id:    m99_erf_erfc_special
 * area:  math_c99
 * funcs: erf erfc erff erfcf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.5.1 (erf), F.10.5.2 (erfc)
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

TC_CASE(erf_zero) { volatile double p = 0.0, n = -0.0; CHECK_BITS_D(erf(p), 0.0); CHECK_BITS_D(erf(n), -0.0); }
TC_CASE(erf_inf) { volatile double i = PINF; CHECK_BITS_D(erf(i), 1.0); CHECK_BITS_D(erf(-i), -1.0); }
TC_CASE(erfc_inf) { volatile double i = PINF; CHECK_BITS_D(erfc(i), 0.0); CHECK_BITS_D(erfc(-i), 2.0); }
TC_CASE(large) { volatile double a = 6, b = -6, c = 30; CHECK_NEAR(erf(a), 1.0, 1e-15); CHECK_NEAR(erfc(b), 2.0, 1e-15); CHECK(erfc(c) < 1e-300); CHECK(erfc(c) >= 0.0); }
TC_CASE(nan_in) { volatile double n = QNAN; volatile float fn = FQNAN; CHECK_IS_NAN(erf(n)); CHECK_IS_NAN(erfc(n)); CHECK_IS_NAN(erff(fn)); CHECK_IS_NAN(erfcf(fn)); }
TC_CASE(float_special) { volatile float p = 0.0f, n = -0.0f, i = FPINF;
    CHECK_BITS_F(erff(p), 0.0f); CHECK_BITS_F(erff(n), -0.0f); CHECK_BITS_F(erff(i), 1.0f); CHECK_BITS_F(erff(-i), -1.0f);
    CHECK_BITS_F(erfcf(i), 0.0f); CHECK_BITS_F(erfcf(-i), 2.0f); }

int main(void) { tc_begin(); TC_RUN(erf_zero); TC_RUN(erf_inf); TC_RUN(erfc_inf); TC_RUN(large); TC_RUN(nan_in); TC_RUN(float_special); return tc_end(); }
