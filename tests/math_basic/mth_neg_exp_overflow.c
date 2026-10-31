/* TC
 * id:    mth_neg_exp_overflow
 * area:  math_basic
 * funcs: exp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs exp (overflow returns INF, errno ERANGE)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(e710) { double r; errno = 0; r = exp(D(710.0)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(e1e10) { double r; errno = 0; r = exp(D(1e10)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(e709) { double r; errno = 0; r = exp(D(709.0)); CHECK(r > 8e307 && r < 9e307); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(e710); TC_RUN(e1e10); TC_RUN(e709); return tc_end(); }
