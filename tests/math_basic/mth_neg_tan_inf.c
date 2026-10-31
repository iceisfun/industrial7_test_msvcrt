/* TC
 * id:    mth_neg_tan_inf
 * area:  math_basic
 * funcs: tan
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs tan (tan(+-INF) returns invalid, errno EDOM); C11 F.10.1.7
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(pos) { double r; errno = 0; r = tan(INF()); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(neg) { double r; errno = 0; r = tan(-INF()); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }

int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg); return tc_end(); }
