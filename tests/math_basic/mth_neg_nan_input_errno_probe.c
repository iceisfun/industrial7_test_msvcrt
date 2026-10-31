/* TC
 * id:    mth_neg_nan_input_errno_probe
 * area:  math_basic
 * funcs: sin log sqrt exp asin fmod
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.1 (domain error only for non-NaN args)
 * note:  Probe: quiet NaN operands do not set errno (stays 0) in sin/log/sqrt/exp/asin/fmod; old CRTs may set EDOM.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double pinf(void) { return v_one / v_zero; }
static void note_errno(const char *what, int e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

static double qnan(void) { return pinf() - pinf(); }
TC_CASE(sin_nan) { volatile double n = qnan(), one = 1.0; double r; (void)one; errno = 0; r = sin(n); CHECK(r != r); CHECK_EQ_I(errno, 0); }
TC_CASE(log_nan) { volatile double n = qnan(), one = 1.0; double r; (void)one; errno = 0; r = log(n); CHECK(r != r); CHECK_EQ_I(errno, 0); }
TC_CASE(sqrt_nan) { volatile double n = qnan(), one = 1.0; double r; (void)one; errno = 0; r = sqrt(n); CHECK(r != r); CHECK_EQ_I(errno, 0); }
TC_CASE(exp_nan) { volatile double n = qnan(), one = 1.0; double r; (void)one; errno = 0; r = exp(n); CHECK(r != r); CHECK_EQ_I(errno, 0); }
TC_CASE(asin_nan) { volatile double n = qnan(), one = 1.0; double r; (void)one; errno = 0; r = asin(n); CHECK(r != r); CHECK_EQ_I(errno, 0); }
TC_CASE(fmod_nan) { volatile double n = qnan(), one = 1.0; double r; (void)one; errno = 0; r = fmod(n, one); CHECK(r != r); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(sin_nan); TC_RUN(log_nan); TC_RUN(sqrt_nan); TC_RUN(exp_nan); TC_RUN(asin_nan); TC_RUN(fmod_nan); return tc_end(); }
