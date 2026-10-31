/* TC
 * id:    mth_neg_isnan_rejects_numbers
 * area:  math_basic
 * funcs: _isnan
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _isnan ("returns a nonzero value if the argument is a NaN, otherwise 0")
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(zero) { volatile double x = 0.0, y = -0.0; CHECK_EQ_I(_isnan(x), 0); CHECK_EQ_I(_isnan(y), 0); }
TC_CASE(infs) { volatile double x = mk(0x7FF0000000000000ULL), y = mk(0xFFF0000000000000ULL); CHECK_EQ_I(_isnan(x), 0); CHECK_EQ_I(_isnan(y), 0); }
TC_CASE(dbl_max) { volatile double x = DBL_MAX, y = -DBL_MAX; CHECK_EQ_I(_isnan(x), 0); CHECK_EQ_I(_isnan(y), 0); }
TC_CASE(denorm) { volatile double x = mk(1ULL), y = mk(0x000FFFFFFFFFFFFFULL); CHECK_EQ_I(_isnan(x), 0); CHECK_EQ_I(_isnan(y), 0); }

int main(void) { tc_begin(); TC_RUN(zero); TC_RUN(infs); TC_RUN(dbl_max); TC_RUN(denorm); return tc_end(); }
