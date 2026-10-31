/* TC
 * id:    mth_j1_basic
 * area:  math_basic
 * funcs: _j1
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _j0, _j1, _jn, _y0, _y1, _yn
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;

static double J(double x) { volatile double v = x; return _j1(v); }
TC_CASE(zero) { CHECK_BITS_D(J(0.0), 0.0); }
TC_CASE(one) { CHECK_NEAR(J(1.0), 0.44005058574493355, 1e-12); }
TC_CASE(odd) { CHECK_NEAR(J(-1.0), -J(1.0), 1e-15); }
TC_CASE(first_root) { CHECK_NEAR(J(3.8317059702075125), 0.0, 1e-9); }

int main(void) { tc_begin(); TC_RUN(zero); TC_RUN(one); TC_RUN(odd); TC_RUN(first_root); return tc_end(); }
