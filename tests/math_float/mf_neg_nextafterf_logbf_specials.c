/* TC
 * id:    mf_neg_nextafterf_logbf_specials
 * area:  math_float
 * funcs:
 * procs: _nextafterf _logbf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.11.3 + F.10.8.3 nextafter (overflow -> inf, NaN propagates, nextafter(x,x)=x), F.10.3.11 logb(+-0)=-inf, logb(+-inf)=+inf; MS docs _logb, _nextafter
 */
#include "tc.h"
#include <float.h>
TC_DECL_PROC(float, _nextafterf, (float, float)); TC_DECL_PROC(float, _logbf, (float));
static volatile float v0 = 0.0f, v1 = 1.0f, vinf, vnan, vmax = FLT_MAX, v3 = 3.0f;

TC_CASE(logb_zero) { if (!tc_p__logbf) TC_SKIP("absent"); CHECK_BITS_F(tc_p__logbf(v0), -vinf); CHECK_BITS_F(tc_p__logbf(-v0), -vinf); }
TC_CASE(logb_inf) { if (!tc_p__logbf) TC_SKIP("absent"); CHECK_BITS_F(tc_p__logbf(vinf), vinf); CHECK_BITS_F(tc_p__logbf(-vinf), vinf); }
TC_CASE(logb_nan) { if (!tc_p__logbf) TC_SKIP("absent"); CHECK_IS_NAN(tc_p__logbf(vnan)); }
TC_CASE(nextafter_nan) { if (!tc_p__nextafterf) TC_SKIP("absent"); CHECK_IS_NAN(tc_p__nextafterf(vnan, v1)); CHECK_IS_NAN(tc_p__nextafterf(v1, vnan)); }
TC_CASE(nextafter_overflow) { if (!tc_p__nextafterf) TC_SKIP("absent"); CHECK_BITS_F(tc_p__nextafterf(vmax, vinf), vinf); CHECK_BITS_F(tc_p__nextafterf(-vmax, -vinf), -vinf); }
TC_CASE(nextafter_equal) { if (!tc_p__nextafterf) TC_SKIP("absent"); CHECK_BITS_F(tc_p__nextafterf(v3, v3), 3.0f); }
int main(void)
{
    tc_begin(); vinf = v1 / v0; vnan = vinf - vinf;
    TC_LOAD_PROC(_nextafterf); TC_LOAD_PROC(_logbf);
    TC_RUN(logb_zero); TC_RUN(logb_inf); TC_RUN(logb_nan); TC_RUN(nextafter_nan); TC_RUN(nextafter_overflow); TC_RUN(nextafter_equal);
    return tc_end();
}
