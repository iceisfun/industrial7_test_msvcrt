/* TC
 * id:    mth_neg_ldexp_range_erange
 * area:  math_basic
 * funcs: ldexp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs ldexp (overflow returns +-INF, errno ERANGE)
 * note:  renamed from spec id mth_neg_ldexp_overflow, which already exists in the corpus.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(pos) { double r; errno = 0; r = ldexp(D(1.0), 1024); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg) { double r; errno = 0; r = ldexp(D(-1.0), 2000); CHECK_BITS_D(r, -INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(int_max) { volatile int e = 0x7fffffff; double r = ldexp(D(1.0), e); CHECK_BITS_D(r, INF()); }
TC_CASE(fits) { double r; errno = 0; r = ldexp(D(1.0), 1023); CHECK_BITS_D(r, 0x1p1023); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg); TC_RUN(int_max); TC_RUN(fits); return tc_end(); }
