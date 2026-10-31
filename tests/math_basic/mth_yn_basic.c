/* TC
 * id:    mth_yn_basic
 * area:  math_basic
 * funcs: _yn _y0 _y1
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   MS docs _y0, _y1, _yn; Abramowitz-Stegun 9.1
 */
#include "tc.h"
#include <math.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(matches_y0_y1) { volatile double x=1.0; CHECK_NEAR(_yn(0,x),_y0(x),1e-12); CHECK_NEAR(_yn(1,x),_y1(x),1e-12); }
TC_CASE(order2) { volatile double x=1.0; CHECK_NEAR(_yn(2,x),-1.6506826068162546,1e-11); }
TC_CASE(order0_at2) { volatile double x=2.0; CHECK_NEAR(_yn(0,x),0.5103756726497451,1e-11); }

int main(void) { tc_begin(); TC_RUN(matches_y0_y1); TC_RUN(order2); TC_RUN(order0_at2); return tc_end(); }
