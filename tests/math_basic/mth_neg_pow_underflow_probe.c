/* TC
 * id:    mth_neg_pow_underflow_probe
 * area:  math_basic
 * funcs: pow
 * crt:   msvcrt msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.1 (underflow: ERANGE implementation-defined)
 * note:  probing pow(10,-400) == +0 (value, asserted) and errno == ERANGE (C99 permits it; asserted).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(value) { double r = pow(D(10.0), D(-400.0)); CHECK_BITS_D(r, 0.0); }
TC_CASE(errno_range) { double r; errno = 0; r = pow(D(10.0), D(-400.0)); (void)r; CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(value); TC_RUN(errno_range); return tc_end(); }
