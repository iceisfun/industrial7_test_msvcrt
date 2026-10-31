/* TC
 * id:    mth_neg_finite_rejects_inf_nan
 * area:  math_basic
 * funcs: _finite
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _finite ("returns 0 if the argument is infinite or a NaN")
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(pos_inf) { volatile double x = mk(0x7FF0000000000000ULL); CHECK_EQ_I(_finite(x), 0); }
TC_CASE(neg_inf) { volatile double x = mk(0xFFF0000000000000ULL); CHECK_EQ_I(_finite(x), 0); }
TC_CASE(qnan)    { volatile double x = mk(0x7FF8000000000000ULL); CHECK_EQ_I(_finite(x), 0); }
TC_CASE(neg_nan) { volatile double x = mk(0xFFF8000000000000ULL); CHECK_EQ_I(_finite(x), 0); }

int main(void) { tc_begin(); TC_RUN(pos_inf); TC_RUN(neg_inf); TC_RUN(qnan); TC_RUN(neg_nan); return tc_end(); }
