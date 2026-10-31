/* TC
 * id:    mth_pow_half_sqrt_probe
 * area:  math_basic
 * funcs: pow sqrt
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs pow
 * note:  Probing whether pow(x,0.5) is bit-identical to sqrt(x) for x in {2,3,10,0.1}; no standard promises this.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double tc_inf(void) { return v_one / v_zero; }
/* result is NaN and errno == err */
#define NAN_ERR(expr, err) do { volatile double tc_r; errno = 0; tc_r = (expr); CHECK_IS_NAN(tc_r); CHECK_EQ_I(errno, err); } while (0)

TC_CASE(pow_half)
{
    static volatile double xs[4] = { 2.0, 3.0, 10.0, 0.1 };
    volatile double h = 0.5; int i;
    errno = 0;
    for (i = 0; i < 4; i++) { volatile double x = xs[i]; CHECK_EQ_U(tc_dbits(pow(x, h)), tc_dbits(sqrt(x))); }
}

int main(void)
{
    tc_begin();
    TC_RUN(pow_half);
    return tc_end();
}
