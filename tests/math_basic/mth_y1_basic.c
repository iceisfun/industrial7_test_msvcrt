/* TC
 * id:    mth_y1_basic
 * area:  math_basic
 * funcs: _y1
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
TC_CASE(values) { volatile double a=1.0,b=2.0; CHECK_NEAR(_y1(a),-0.7812128213002887,1e-12); CHECK_NEAR(_y1(b),-0.10703243154093755,1e-12); }
TC_CASE(first_zero) { volatile double x=2.197141326031017; CHECK_NEAR(_y1(x),0.0,1e-9); }

int main(void) { tc_begin(); TC_RUN(values); TC_RUN(first_zero); return tc_end(); }
