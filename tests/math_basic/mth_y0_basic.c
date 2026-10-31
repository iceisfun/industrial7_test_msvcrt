/* TC
 * id:    mth_y0_basic
 * area:  math_basic
 * funcs: _y0
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
TC_CASE(values) { volatile double a=1.0,b=2.0; CHECK_NEAR(_y0(a),0.08825696421567697,1e-12); CHECK_NEAR(_y0(b),0.5103756726497451,1e-12); }
TC_CASE(first_zero) { volatile double x=0.893576966279167; CHECK_NEAR(_y0(x),0.0,1e-9); }

int main(void) { tc_begin(); TC_RUN(values); TC_RUN(first_zero); return tc_end(); }
