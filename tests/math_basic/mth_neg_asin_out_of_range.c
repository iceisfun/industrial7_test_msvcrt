/* TC
 * id:    mth_neg_asin_out_of_range
 * area:  math_basic
 * funcs: asin
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs asin: |x| > 1 -> NaN, EDOM
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double tc_inf(void) { return v_one / v_zero; }
/* result is NaN and errno == err */
#define NAN_ERR(expr, err) do { volatile double tc_r; errno = 0; tc_r = (expr); CHECK_IS_NAN(tc_r); CHECK_EQ_I(errno, err); } while (0)

TC_CASE(two)
{
    volatile double x = 2.0;
    NAN_ERR(asin(x), EDOM);
}

TC_CASE(just_below_minus_one)
{
    volatile double x = -1.0000000000000002;
    NAN_ERR(asin(x), EDOM);
}

TC_CASE(inf)
{
    volatile double x = tc_inf();
    NAN_ERR(asin(x), EDOM);
}

TC_CASE(boundary_no_errno)
{
    volatile double x = 1.0, r;
    errno = 0; r = asin(x);
    CHECK_NEAR(r, 1.5707963267948966, 1e-15);
    CHECK_EQ_I(errno, 0);
}

int main(void)
{
    tc_begin();
    TC_RUN(two);
    TC_RUN(just_below_minus_one);
    TC_RUN(inf);
    TC_RUN(boundary_no_errno);
    return tc_end();
}
