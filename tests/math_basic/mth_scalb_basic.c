/* TC
 * id:    mth_scalb_basic
 * area:  math_basic
 * funcs: _scalb
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   MS docs _scalb
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(positive) { volatile double a=1.0; volatile long n=3; CHECK_BITS_D(_scalb(a,n),8.0); }
TC_CASE(negative_exp) { volatile double a=3.0; volatile long n=-1; CHECK_BITS_D(_scalb(a,n),1.5); }
TC_CASE(negative_x) { volatile double a=-2.0; volatile long n=2; CHECK_BITS_D(_scalb(a,n),-8.0); }
TC_CASE(zero) { volatile double a=0.0; volatile long n=100; CHECK_BITS_D(_scalb(a,n),0.0); }
TC_CASE(exp0) { volatile double a=1.5; volatile long n=0; CHECK_BITS_D(_scalb(a,n),1.5); }

int main(void) { tc_begin(); TC_RUN(positive); TC_RUN(negative_exp); TC_RUN(negative_x); TC_RUN(zero); TC_RUN(exp0); return tc_end(); }
