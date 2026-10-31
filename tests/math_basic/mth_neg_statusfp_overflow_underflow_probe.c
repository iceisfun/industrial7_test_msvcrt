/* TC
 * id:    mth_neg_statusfp_overflow_underflow_probe
 * area:  math_basic
 * funcs: _statusfp _clearfp
 * procs: exp pow
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _statusfp, _clearfp (_SW_OVERFLOW 0x04, _SW_UNDERFLOW 0x02, _SW_INEXACT 0x01)
 * note:  PROBE: masked overflow/underflow in exp/pow raise the sticky status flags. exp(-1000) => UNDERFLOW is the least certain
 *        (printed in its own case). INEXACT is ignored everywhere.
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
typedef double (__cdecl *d2_t)(double, double);
static d1_t p_exp; static d2_t p_pow;
#define exp p_exp
#define pow p_pow
#define SW_OVERFLOW 0x04u
#define SW_UNDERFLOW 0x02u
TC_CASE(exp_over) { volatile double x = 1000.0; double r; _clearfp(); r = exp(x); CHECK_BITS_D(r, pinf()); CHECK(_statusfp() & SW_OVERFLOW); _clearfp(); }
TC_CASE(exp_under_info) { volatile double x = -1000.0; double r; unsigned s; _clearfp(); r = exp(x); CHECK_BITS_D(r, 0.0); s = _statusfp(); info_i("exp_under_underflow_flag", (s & SW_UNDERFLOW) != 0); _clearfp(); }
TC_CASE(exp_under_flag) { volatile double x = -1000.0; double r; _clearfp(); r = exp(x); (void)r; CHECK(_statusfp() & SW_UNDERFLOW); _clearfp(); }
TC_CASE(pow_over) { volatile double b = 2.0, e = 2000.0; double r; _clearfp(); r = pow(b, e); CHECK_BITS_D(r, pinf()); CHECK(_statusfp() & SW_OVERFLOW); _clearfp(); }
TC_CASE(exp_one) { volatile double x = 1.0; double r; _clearfp(); r = exp(x); CHECK_NEAR(r, 2.718281828459045, 1e-14); CHECK(!(_statusfp() & (SW_OVERFLOW | SW_UNDERFLOW))); _clearfp(); }

int main(void) { tc_begin(); p_exp = (d1_t)tc_crt_proc("exp"); p_pow = (d2_t)tc_crt_proc("pow"); if (!p_exp || !p_pow) return 1; TC_RUN(exp_over); TC_RUN(exp_under_info); TC_RUN(exp_under_flag); TC_RUN(pow_over); TC_RUN(exp_one); return tc_end(); }
