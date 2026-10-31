/* TC
 * id:    mth_neg_log_zero
 * area:  math_basic
 * funcs: log
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs log: x == 0 returns -INF, errno ERANGE
 * note:  -INF value asserted on all CRTs; errno==ERANGE asserted on ucrtbase only (pre-UCRT may set EDOM for a
 *        singularity; unverified)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double tc_inf(void) { return v_one / v_zero; }
/* result is NaN and errno == err */
#define NAN_ERR(expr, err) do { volatile double tc_r; errno = 0; tc_r = (expr); CHECK_IS_NAN(tc_r); CHECK_EQ_I(errno, err); } while (0)

TC_CASE(plus_zero)
{
    volatile double x = 0.0, r;
    errno = 0; r = log(x);
    CHECK(r < 0 && r == -tc_inf());
    if (TC_AT_LEAST(140)) CHECK_EQ_I(errno, ERANGE);
}

TC_CASE(minus_zero_value)
{
    volatile double x = -0.0, r;
    r = log(x);
    CHECK(r < 0 && r == -tc_inf());
}

TC_CASE(minus_zero_errno)
{
    volatile double x = -0.0, r;
    errno = 0; r = log(x);
    CHECK(r == -tc_inf());
    if (TC_AT_LEAST(140)) CHECK_EQ_I(errno, ERANGE);
}

int main(void)
{
    tc_begin();
    TC_RUN(plus_zero);
    TC_RUN(minus_zero_value);
    TC_RUN(minus_zero_errno);
    return tc_end();
}
