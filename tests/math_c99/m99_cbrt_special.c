/* TC
 * id:    m99_cbrt_special
 * area:  math_c99
 * funcs: cbrt cbrtf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.4.1
 */
#include "tc.h"
#include <math.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;

TC_CASE(zeros) { volatile double p = 0.0, n = -0.0; CHECK_BITS_D(cbrt(p), 0.0); CHECK_BITS_D(cbrt(n), -0.0); }
TC_CASE(infs) { volatile double i = v_one / v_zero; CHECK_BITS_D(cbrt(i), i); CHECK_BITS_D(cbrt(-i), -i); }
TC_CASE(nan_in) { volatile double i = v_one / v_zero; volatile double n = i - i; CHECK_IS_NAN(cbrt(n)); }
TC_CASE(denormal) { volatile double d = 4.9406564584124654e-324; double r = cbrt(d); CHECK(r > 0.0); CHECK_NEAR(r, 1.7e-108, 1e-109); }
TC_CASE(float_special)
{
    volatile float p = 0.0f, n = -0.0f; volatile float i = f_one / f_zero;
    CHECK_BITS_F(cbrtf(p), 0.0f); CHECK_BITS_F(cbrtf(n), -0.0f); CHECK_BITS_F(cbrtf(i), i); CHECK_BITS_F(cbrtf(-i), -i);
}

int main(void) { tc_begin(); TC_RUN(zeros); TC_RUN(infs); TC_RUN(nan_in); TC_RUN(denormal); TC_RUN(float_special); return tc_end(); }
