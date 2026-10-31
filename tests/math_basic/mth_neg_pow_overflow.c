/* TC
 * id:    mth_neg_pow_overflow
 * area:  math_basic
 * funcs: pow
 * crt:   msvcrt msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs pow (overflow returns +-HUGE_VAL, errno ERANGE)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(p10) { double r; errno = 0; r = pow(D(10.0), D(400.0)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(p2) { double r; errno = 0; r = pow(D(2.0), D(1024.0)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg) { double r; errno = 0; r = pow(D(-2.0), D(1025.0)); CHECK_BITS_D(r, -INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(fits) { double r; errno = 0; r = pow(D(2.0), D(1023.0)); CHECK(r > 8.9e307 && r < 9.1e307); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(p10); TC_RUN(p2); TC_RUN(neg); TC_RUN(fits); return tc_end(); }
