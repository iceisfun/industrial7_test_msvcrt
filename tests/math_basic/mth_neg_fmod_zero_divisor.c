/* TC
 * id:    mth_neg_fmod_zero_divisor
 * area:  math_basic
 * funcs: fmod
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fmod (y==0 or x infinite: NaN, errno EDOM); C11 F.10.7.1
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(one_zero) { double r; errno = 0; r = fmod(D(1.0), D(0.0)); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(negzero) { double r; errno = 0; r = fmod(D(-1.0), D(-0.0)); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(inf_x) { double r; errno = 0; r = fmod(INF(), D(1.0)); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }

int main(void) { tc_begin(); TC_RUN(one_zero); TC_RUN(negzero); TC_RUN(inf_x); return tc_end(); }
