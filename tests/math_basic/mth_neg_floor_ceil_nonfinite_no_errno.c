/* TC
 * id:    mth_neg_floor_ceil_nonfinite_no_errno
 * area:  math_basic
 * funcs: floor ceil _copysign _chgsign
 * procs: fabs
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.6.1-2, F.10.4.3 fabs (non-finite args are not errors); MS docs floor, ceil, fabs, _copysign, _chgsign
 * note:  Non-finite arguments to rounding/sign functions are not domain errors: errno must stay 0.
 *        modf with non-finite args is covered (as a probe) by mth_modf_special / mth_modf_signed_zero_probe.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_nan_src;
static double pinf(void) { return v_one / v_zero; }
static double qnan(void) { return v_zero / v_zero; }
static int sgn(double d) { return (int)(tc_dbits(d) >> 63); }
static void info_i(const char *what, long long v) { char b[32]; tc_i64(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

typedef double (__cdecl *d1_t)(double);
static d1_t p_fabs;
TC_CASE(floor_inf) { volatile double p = pinf(), m = -pinf(); errno = 0; CHECK_BITS_D(floor(p), pinf()); CHECK_BITS_D(floor(m), -pinf()); CHECK_EQ_I(errno, 0); }
TC_CASE(ceil_nan) { volatile double n = qnan(); errno = 0; CHECK_IS_NAN(ceil(n)); CHECK_EQ_I(errno, 0); }
TC_CASE(fabs_inf) { volatile double m = -pinf(); errno = 0; CHECK_BITS_D(p_fabs(m), pinf()); CHECK_EQ_I(errno, 0); }
TC_CASE(copysign_nan) { volatile double n = qnan(), s = -1.0; double r; errno = 0; r = _copysign(n, s); CHECK_IS_NAN(r); CHECK_EQ_I(sgn(r), 1); CHECK_EQ_I(errno, 0); }
TC_CASE(chgsign_inf) { volatile double p = pinf(); errno = 0; CHECK_BITS_D(_chgsign(p), -pinf()); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); p_fabs = (d1_t)tc_crt_proc("fabs"); if (!p_fabs) return 1; TC_RUN(floor_inf); TC_RUN(ceil_nan); TC_RUN(fabs_inf); TC_RUN(copysign_nan); TC_RUN(chgsign_inf); return tc_end(); }
