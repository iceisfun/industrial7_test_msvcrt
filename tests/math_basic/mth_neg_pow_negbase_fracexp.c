/* TC
 * id:    mth_neg_pow_negbase_fracexp
 * area:  math_basic
 * funcs: pow
 * crt:   msvcrt msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs pow (x<0 and y non-integer: invalid, NaN, errno EDOM)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(cube_root) { double r; errno = 0; r = pow(D(-8.0), D(1.0) / D(3.0)); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(half) { double r; errno = 0; r = pow(D(-1.0), D(0.5)); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(negfrac) { double r; errno = 0; r = pow(D(-2.0), D(-0.5)); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(valid_int) { double r; errno = 0; r = pow(D(-8.0), D(3.0)); CHECK_BITS_D(r, -512.0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(cube_root); TC_RUN(half); TC_RUN(negfrac); TC_RUN(valid_int); return tc_end(); }
