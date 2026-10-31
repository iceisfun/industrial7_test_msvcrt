/* TC
 * id:    m99_finite_isnan_legacy
 * area:  math_c99
 * funcs: _finite _isnan
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _finite, _isnan (float.h)
 */
#include "tc.h"
#include <math.h>
#include <float.h>
static volatile double v_zero = 0.0, v_one = 1.0, v_max = DBL_MAX, v_min = 4.9406564584124654e-324;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
TC_CASE(finite) { CHECK(_finite(v_zero) != 0); CHECK(_finite(v_max) != 0); CHECK(_finite(d_inf()) == 0); CHECK(_finite(-d_inf()) == 0); CHECK(_finite(d_nan()) == 0); }
TC_CASE(isnan) { CHECK(_isnan(d_nan()) != 0); CHECK(_isnan(-d_nan()) != 0); CHECK(_isnan(d_inf()) == 0); CHECK(_isnan(v_one) == 0); }
TC_CASE(denormal) { CHECK(_finite(v_min) != 0); CHECK(_isnan(v_min) == 0); }
int main(void) { tc_begin(); TC_RUN(finite); TC_RUN(isnan); TC_RUN(denormal); return tc_end(); }
