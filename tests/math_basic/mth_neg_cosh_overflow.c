/* TC
 * id:    mth_neg_cosh_overflow
 * area:  math_basic
 * funcs: cosh
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs cosh (overflow returns HUGE_VAL, errno ERANGE)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(pos) { double r; errno = 0; r = cosh(D(1000.0)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg) { double r; errno = 0; r = cosh(D(-1000.0)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg); return tc_end(); }
