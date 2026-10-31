/* TC
 * id:    m99_scalbn_neg_range
 * area:  math_c99
 * funcs: scalbn scalbln scalbnf scalblnf
 * procs: 
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.6.13, F.10.3.13 (overflow/underflow)
 * note:  errno asserted only in {0,ERANGE}
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#define ERR_OK() (errno == 0 || errno == ERANGE)
static volatile double v_zero = 0.0, v_one = 1.0;
TC_CASE(overflow) { volatile double a = 1, b = -1; double inf = v_one / v_zero; errno = 0; CHECK(scalbn(a, 1024) == inf); CHECK(ERR_OK()); errno = 0; CHECK(scalbn(b, 1024) == -inf); CHECK(ERR_OK()); }
TC_CASE(int_extremes) { volatile double a = 1; volatile int hi = INT_MAX, lo = INT_MIN; double inf = v_one / v_zero;
    errno = 0; CHECK(scalbn(a, hi) == inf); CHECK(ERR_OK()); errno = 0; CHECK_BITS_D(scalbn(a, lo), 0.0); CHECK(ERR_OK()); }
TC_CASE(long_extremes) { volatile double a = 1; volatile long hi = LONG_MAX, lo = LONG_MIN; double inf = v_one / v_zero;
    errno = 0; CHECK(scalbln(a, hi) == inf); CHECK(ERR_OK()); errno = 0; CHECK_BITS_D(scalbln(a, lo), 0.0); CHECK(ERR_OK()); }
TC_CASE(float_overflow) { volatile float a = 1; float inf = (float)(v_one / v_zero); errno = 0; CHECK(scalbnf(a, 128) == inf); CHECK(ERR_OK()); errno = 0; CHECK(scalblnf(a, 200) == inf); CHECK(ERR_OK()); }
int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(int_extremes); TC_RUN(long_extremes); TC_RUN(float_overflow); return tc_end(); }
