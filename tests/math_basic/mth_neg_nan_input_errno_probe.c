/* TC
 * id:    mth_neg_nan_input_errno_probe
 * area:  math_basic
 * funcs:
 * procs: sin log sqrt exp asin fmod
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.12.1 (domain error only for non-NaN args)
 * note:  Calls go to the real ucrtbase export via GetProcAddress (a bare call in a mingw exe lands in libmingwex static code, not the DLL). Reference: all six leave errno 0 for qNaN input.
 *        Probe: quiet NaN operands do not set errno (stays 0) in sin/log/sqrt/exp/asin/fmod; old CRTs may set EDOM.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
TC_DECL_PROC(double, sin, (double));
#undef sin
#define sin tc_p_sin
TC_DECL_PROC(double, log, (double));
#undef log
#define log tc_p_log
TC_DECL_PROC(double, sqrt, (double));
#undef sqrt
#define sqrt tc_p_sqrt
TC_DECL_PROC(double, exp, (double));
#undef exp
#define exp tc_p_exp
TC_DECL_PROC(double, asin, (double));
#undef asin
#define asin tc_p_asin
TC_DECL_PROC(double, fmod, (double, double));
#undef fmod
#define fmod tc_p_fmod

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

int main(void) { tc_begin(); TC_LOAD_PROC(sin); if (!tc_p_sin) { tc_write("TC FAIL " TC_TEST_ID "::proc sin not exported\n"); return 1; } TC_LOAD_PROC(log); if (!tc_p_log) { tc_write("TC FAIL " TC_TEST_ID "::proc log not exported\n"); return 1; } TC_LOAD_PROC(sqrt); if (!tc_p_sqrt) { tc_write("TC FAIL " TC_TEST_ID "::proc sqrt not exported\n"); return 1; } TC_LOAD_PROC(exp); if (!tc_p_exp) { tc_write("TC FAIL " TC_TEST_ID "::proc exp not exported\n"); return 1; } TC_LOAD_PROC(asin); if (!tc_p_asin) { tc_write("TC FAIL " TC_TEST_ID "::proc asin not exported\n"); return 1; } TC_LOAD_PROC(fmod); if (!tc_p_fmod) { tc_write("TC FAIL " TC_TEST_ID "::proc fmod not exported\n"); return 1; } TC_RUN(sin_nan); TC_RUN(log_nan); TC_RUN(sqrt_nan); TC_RUN(exp_nan); TC_RUN(asin_nan); TC_RUN(fmod_nan); return tc_end(); }
