/* TC
 * id:    mth_neg_sqrt_minus_inf
 * area:  math_basic
 * funcs: sqrt
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sqrt; C11 F.10.4.5
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double tc_inf(void) { return v_one / v_zero; }
/* result is NaN and errno == err */
#define NAN_ERR(expr, err) do { volatile double tc_r; errno = 0; tc_r = (expr); CHECK_IS_NAN(tc_r); CHECK_EQ_I(errno, err); } while (0)

TC_CASE(minus_inf)
{
    volatile double x = -tc_inf();
    NAN_ERR(sqrt(x), EDOM);
}

int main(void)
{
    tc_begin();
    TC_RUN(minus_inf);
    return tc_end();
}
