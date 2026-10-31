/* TC
 * id:    mth_neg_statusfp_invalid_zerodiv_probe
 * area:  math_basic
 * funcs: _statusfp _clearfp
 * procs: sqrt log
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _statusfp, _clearfp (_SW_INVALID 0x10, _SW_ZERODIVIDE 0x08)
 * note:  PROBE: with exceptions masked (default), domain/pole errors in sqrt/log raise the sticky status flags.
 *        sqrt(-1) and log(-1) => _SW_INVALID; log(0) => _SW_ZERODIVIDE (own case log_zero_flag, least certain; the info case prints which flags were set). sqrt(4) must not raise INVALID.
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
static d1_t p_sqrt, p_log;
#define sqrt p_sqrt
#define log p_log
#define SW_INVALID 0x10u
#define SW_ZERODIVIDE 0x08u
TC_CASE(sqrt_neg) { volatile double x = -1.0; double r; _clearfp(); r = sqrt(x); (void)r; CHECK(_statusfp() & SW_INVALID); _clearfp(); }
TC_CASE(log_neg) { volatile double x = -1.0; double r; _clearfp(); r = log(x); (void)r; CHECK(_statusfp() & SW_INVALID); _clearfp(); }
TC_CASE(log_zero_info) { volatile double x = 0.0; double r; unsigned s; _clearfp(); r = log(x); (void)r; s = _statusfp(); info_i("log0_zerodivide", (s & SW_ZERODIVIDE) != 0); info_i("log0_invalid", (s & SW_INVALID) != 0); _clearfp(); }
TC_CASE(log_zero_flag) { volatile double x = 0.0; double r; _clearfp(); r = log(x); (void)r; CHECK(_statusfp() & SW_ZERODIVIDE); _clearfp(); }
TC_CASE(sqrt_ok) { volatile double x = 4.0; double r; _clearfp(); r = sqrt(x); CHECK_BITS_D(r, 2.0); CHECK(!(_statusfp() & SW_INVALID)); _clearfp(); }
TC_CASE(clear) { volatile double x = -1.0; double r; r = sqrt(x); (void)r; _clearfp(); CHECK_EQ_U(_statusfp() & 0x1fu, 0); }

int main(void) { tc_begin(); p_sqrt = (d1_t)tc_crt_proc("sqrt"); p_log = (d1_t)tc_crt_proc("log"); if (!p_sqrt || !p_log) return 1; TC_RUN(sqrt_neg); TC_RUN(log_neg); TC_RUN(log_zero_info); TC_RUN(log_zero_flag); TC_RUN(sqrt_ok); TC_RUN(clear); return tc_end(); }
