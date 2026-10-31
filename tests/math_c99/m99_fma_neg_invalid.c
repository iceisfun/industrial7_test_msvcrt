/* TC
 * id:    m99_fma_neg_invalid
 * area:  math_c99
 * funcs: fma fmaf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.10.1 (invalid operation returns NaN); MS docs fma
 * note:  errno is not asserted exactly: EDOM or 0 accepted.
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

TC_CASE(zero_times_inf)
{
    volatile double z = 0.0, o = 1.0, i = d_inf();
    errno = 0; CHECK_IS_NAN(fma(z, i, o)); CHECK(errno == 0 || errno == EDOM);
    errno = 0; CHECK_IS_NAN(fma(i, z, z)); CHECK(errno == 0 || errno == EDOM);
}
TC_CASE(float_zero_times_inf)
{
    volatile float z = 0.0f, o = 1.0f, i = f_inf();
    errno = 0; { float r = fmaf(z, i, o); CHECK(r != r); } CHECK(errno == 0 || errno == EDOM);
    errno = 0; { float r = fmaf(i, z, z); CHECK(r != r); } CHECK(errno == 0 || errno == EDOM);
}
TC_CASE(inf_minus_inf)
{
    volatile double o = 1.0, i = d_inf();
    errno = 0; CHECK_IS_NAN(fma(i, o, -i)); CHECK(errno == 0 || errno == EDOM);
}
int main(void) { tc_begin(); TC_RUN(zero_times_inf); TC_RUN(float_zero_times_inf); TC_RUN(inf_minus_inf); return tc_end(); }
