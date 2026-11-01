/* TC
 * id:    mth_modf_signed_zero_probe
 * area:  math_basic
 * funcs:
 * procs: modf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 F.10.3.12
 * note:  PROBE: sign of zero parts from modf (C99 requires the sign of x), modf(+-inf) on every CRT incl. legacy, Observed on an FMA3/AVX2 CPU (ucrtbase dispatches FMA3 paths); non-FMA path unverified.
 *        and modf(NaN) giving NaN in both outputs (not specified by C11 for the integral part).
 */
#include "tc.h"
#include <math.h>
/* F: call the DLL export, never the libmingwex static copy (tc_crt_proc = GetProcAddress) */
TC_DECL_PROC(double, modf, (double, double *));
#undef modf
#define modf tc_p_modf

static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(neg_integral_frac) { volatile double x = -5.0; double ip = -99; double f = modf(x, &ip); CHECK_BITS_D(f, -0.0); CHECK_BITS_D(ip, -5.0); }
TC_CASE(neg_small_ip) { volatile double x = -0.25; double ip = -99; double f = modf(x, &ip); CHECK_BITS_D(f, -0.25); CHECK_BITS_D(ip, -0.0); }
TC_CASE(pos_inf) { volatile double x = v_one / v_zero; double ip = -99; double f = modf(x, &ip); CHECK_BITS_D(f, 0.0); CHECK(ip == x); }
TC_CASE(neg_inf) { volatile double x = -(v_one / v_zero); double ip = -99; double f = modf(x, &ip); CHECK_BITS_D(f, -0.0); CHECK(ip == x); }
TC_CASE(nan_in) { volatile double x = v_zero / v_zero; double ip = -99; double f = modf(x, &ip); CHECK_IS_NAN(f); CHECK_IS_NAN(ip); }

int main(void) { tc_begin(); TC_LOAD_PROC(modf); if (!tc_p_modf) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(neg_integral_frac); TC_RUN(neg_small_ip); TC_RUN(pos_inf); TC_RUN(neg_inf); TC_RUN(nan_in); return tc_end(); }
