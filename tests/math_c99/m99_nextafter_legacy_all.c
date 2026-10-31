/* TC
 * id:    m99_nextafter_legacy_all
 * area:  math_c99
 * funcs: _nextafter _copysign _logb _scalb _finite _chgsign
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _nextafter, _copysign, _logb, _scalb, _finite, _isnan, _chgsign
 * note:  _isnan returns nonzero, not necessarily 1.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }

TC_CASE(nextafter)
{
    volatile double one = 1.0, two = 2.0;
    CHECK_BITS_D(_nextafter(one, two), 1.0 + DBL_EPSILON);
    CHECK_BITS_D(_nextafter(one, -two), 1.0 - DBL_EPSILON / 2);
}
TC_CASE(copysign) { volatile double a = 3.0, nz = -0.0; CHECK_BITS_D(_copysign(a, nz), -3.0); CHECK_BITS_D(_copysign(-a, 1.0), 3.0); }
TC_CASE(logb_scalb)
{
    volatile double e = 8.0, one = 1.0; volatile long four = 4;
    CHECK_BITS_D(_logb(e), 3.0); CHECK_BITS_D(_scalb(one, four), 16.0);
}
TC_CASE(finite_isnan)
{
    volatile double one = 1.0, i = d_inf(), n = d_nan();
    CHECK(_finite(one) != 0); CHECK(_finite(i) == 0); CHECK(_finite(n) == 0);
    CHECK(_isnan(n) != 0); CHECK(_isnan(one) == 0);
}
TC_CASE(chgsign) { volatile double a = 1.5, z = 0.0; CHECK_BITS_D(_chgsign(a), -1.5); CHECK_BITS_D(_chgsign(-a), 1.5); (void)z; }
int main(void) { tc_begin(); TC_RUN(nextafter); TC_RUN(copysign); TC_RUN(logb_scalb); TC_RUN(finite_isnan); TC_RUN(chgsign); return tc_end(); }
