/* TC
 * id:    m99_asinh_special
 * area:  math_c99
 * funcs: asinh asinhf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.2.3; MS docs asinh

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

TC_CASE(zeros) { CHECK_BITS_D(asinh(0.0 * v_one), 0.0); CHECK_BITS_D(asinh(-v_zero * v_one), -0.0 * v_one); }
TC_CASE(infs) { CHECK_BITS_D(asinh(d_inf()), d_inf()); CHECK_BITS_D(asinh(-d_inf()), -d_inf()); }
TC_CASE(nan) { CHECK_IS_NAN(asinh(d_nan())); }
TC_CASE(dbl_max) { volatile double x = DBL_MAX; CHECK_NEAR(asinh(x), 710.4758600739439, 1e-9); }
TC_CASE(floatinf) { CHECK_BITS_F(asinhf(f_inf()), f_inf()); CHECK_BITS_F(asinhf(-f_inf()), -f_inf()); }
int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(infs); TC_RUN(nan); TC_RUN(dbl_max); TC_RUN(floatinf); return tc_end(); }
