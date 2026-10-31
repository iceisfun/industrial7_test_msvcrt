/* TC
 * id:    mth_logb_basic
 * area:  math_basic
 * funcs: _logb
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   MS docs _logb: unbiased exponent; C99 logb
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(pow2) { volatile double a=1.0,b=8.0,c=0.5; CHECK_BITS_D(_logb(a),0.0); CHECK_BITS_D(_logb(b),3.0); CHECK_BITS_D(_logb(c),-1.0); }
TC_CASE(non_pow2) { volatile double a=10.0,b=1023.99; CHECK_BITS_D(_logb(a),3.0); CHECK_BITS_D(_logb(b),9.0); }
TC_CASE(negative) { volatile double a=-1024.0; CHECK_BITS_D(_logb(a),10.0); }
TC_CASE(dbl_min) { volatile double a=DBL_MIN; CHECK_BITS_D(_logb(a),-1022.0); }

int main(void) { tc_begin(); TC_RUN(pow2); TC_RUN(non_pow2); TC_RUN(negative); TC_RUN(dbl_min); return tc_end(); }
