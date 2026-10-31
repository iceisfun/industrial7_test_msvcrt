/* TC
 * id:    m99_neg_log2_exp2_nan_errno_probe
 * area:  math_c99
 * funcs: log2 log2f exp2 exp2f
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.1 (errno use is implementation-defined); MS docs log2, exp2
 * note:  PROBE: a quiet-NaN argument is not a domain error, so errno is assumed to be left unchanged (sentinel 9) by log2/log2f/exp2/exp2f.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0;
#define DNAN (v_zero / v_zero)
#define SENT 9

TC_CASE(double_nan)
{
    volatile double n = DNAN; double r;
    errno = SENT; r = log2(n); CHECK(r != r); CHECK_EQ_I(errno, SENT);
    errno = SENT; r = exp2(n); CHECK(r != r); CHECK_EQ_I(errno, SENT);
}
TC_CASE(float_nan)
{
    volatile float n = (float)DNAN; float r;
    errno = SENT; r = log2f(n); CHECK(r != r); CHECK_EQ_I(errno, SENT);
    errno = SENT; r = exp2f(n); CHECK(r != r); CHECK_EQ_I(errno, SENT);
}
int main(void) { tc_begin(); TC_RUN(double_nan); TC_RUN(float_nan); return tc_end(); }
