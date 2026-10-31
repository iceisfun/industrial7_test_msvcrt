/* TC
 * id:    mth_jn_basic
 * area:  math_basic
 * funcs: _jn _j0 _j1
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   MS docs _j0, _j1, _jn; Abramowitz-Stegun 9.1
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(at_zero) { volatile double z=0.0; CHECK_BITS_D(_jn(0,z),1.0); CHECK_BITS_D(_jn(1,z),0.0); CHECK_BITS_D(_jn(2,z),0.0); }
TC_CASE(matches_j0_j1) { volatile double x=1.0; CHECK_NEAR(_jn(0,x),_j0(x),1e-12); CHECK_NEAR(_jn(1,x),_j1(x),1e-12); }
TC_CASE(order2) { volatile double x=1.0; CHECK_NEAR(_jn(2,x),0.11490348493190049,1e-12); }

int main(void) { tc_begin(); TC_RUN(at_zero); TC_RUN(matches_j0_j1); TC_RUN(order2); return tc_end(); }
