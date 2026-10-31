/* TC
 * id:    m99_asinh_basic
 * area:  math_c99
 * funcs: asinh asinhf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.5.2, F.10.2.3; MS docs asinh

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

TC_CASE(zero) { CHECK_BITS_D(asinh(v_zero), 0.0); }
TC_CASE(one) { CHECK_NEAR(asinh(v_one), 0.881373587019543, 1e-15); CHECK_BITS_D(asinh(-v_one), -asinh(v_one)); }
TC_CASE(tiny) { volatile double x = 1e-10; CHECK_NEAR(asinh(x), 1e-10, 1e-25); }
TC_CASE(huge) { volatile double x = 1e300; CHECK_NEAR(asinh(x), 691.4686750787736, 1e-10); }
TC_CASE(floatv) { CHECK_NEAR(asinhf(f_one), 0.8813736f, 1e-6); }
int main(void) { tc_begin(); TC_RUN(zero); TC_RUN(one); TC_RUN(tiny); TC_RUN(huge); TC_RUN(floatv); return tc_end(); }
