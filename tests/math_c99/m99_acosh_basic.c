/* TC
 * id:    m99_acosh_basic
 * area:  math_c99
 * funcs: acosh acoshf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.5.1, F.10.2.1; MS docs acosh

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

TC_CASE(one) { CHECK_BITS_D(acosh(v_one), 0.0); }
TC_CASE(vals) { volatile double a = 2.0, b = 10.0; CHECK_NEAR(acosh(a), 1.3169578969248166, 1e-15); CHECK_NEAR(acosh(b), 2.993222846126381, 1e-14); }
TC_CASE(inf) { CHECK_BITS_D(acosh(d_inf()), d_inf()); }
TC_CASE(huge) { volatile double x = 1e300; CHECK_NEAR(acosh(x), 691.4686750787736, 1e-10); }
TC_CASE(floatv) { volatile float x = 2.0f; CHECK_NEAR(acoshf(x), 1.316958f, 1e-6); }
int main(void) { tc_begin(); TC_RUN(one); TC_RUN(vals); TC_RUN(inf); TC_RUN(huge); TC_RUN(floatv); return tc_end(); }
