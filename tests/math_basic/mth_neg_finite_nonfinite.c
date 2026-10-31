/* TC
 * id:    mth_neg_finite_nonfinite
 * area:  math_basic
 * funcs: _finite
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   MS docs _finite: returns 0 if infinite or NaN
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(inf_nan) { volatile double i=v_one/v_zero; volatile double n=mk(0x7FF8000000000000ULL); CHECK_EQ_I(_finite(i),0); CHECK_EQ_I(_finite(-i),0); CHECK_EQ_I(_finite(n),0); }

int main(void) { tc_begin(); TC_RUN(inf_nan); return tc_end(); }
