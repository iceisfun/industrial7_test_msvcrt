/* TC
 * id:    mth_neg_log_negative
 * area:  math_basic
 * funcs: log
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs log: negative x -> NaN, EDOM
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double tc_inf(void) { return v_one / v_zero; }
/* result is NaN and errno == err */
#define NAN_ERR(expr, err) do { volatile double tc_r; errno = 0; tc_r = (expr); CHECK_IS_NAN(tc_r); CHECK_EQ_I(errno, err); } while (0)

TC_CASE(minus_one)
{
    volatile double x = -1.0;
    NAN_ERR(log(x), EDOM);
}

TC_CASE(tiny_negative)
{
    volatile double x = -1e-300;
    NAN_ERR(log(x), EDOM);
}

TC_CASE(minus_inf)
{
    volatile double x = -tc_inf();
    NAN_ERR(log(x), EDOM);
}

int main(void)
{
    tc_begin();
    TC_RUN(minus_one);
    TC_RUN(tiny_negative);
    TC_RUN(minus_inf);
    return tc_end();
}
