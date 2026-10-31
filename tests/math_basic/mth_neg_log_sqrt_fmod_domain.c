/* TC
 * id:    mth_neg_log_sqrt_fmod_domain
 * area:  math_basic
 * funcs:
 * procs: log log10 sqrt fmod
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs log, log10, sqrt, fmod (domain error: NaN + errno EDOM; log(0): -inf + ERANGE); C11 7.12.1, F.10
 * note:  domain/pole errors for log, log10, sqrt, fmod
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(double, log, (double));
#define log tc_p_log
TC_DECL_PROC(double, log10, (double));
#define log10 tc_p_log10
TC_DECL_PROC(double, sqrt, (double));
#define sqrt tc_p_sqrt
TC_DECL_PROC(double, fmod, (double, double));
#define fmod tc_p_fmod

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(log_negative) { volatile double x = -1.0; errno = 0; CHECK_IS_NAN(log(x)); CHECK_EQ_I(errno, EDOM); }
TC_CASE(log_zero) { volatile double x = 0.0; errno = 0; CHECK(log(x) == -v_inf); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(log10_negative) { volatile double x = -10.0; errno = 0; CHECK_IS_NAN(log10(x)); CHECK_EQ_I(errno, EDOM); }
TC_CASE(sqrt_negative) { volatile double x = -1.0; errno = 0; CHECK_IS_NAN(sqrt(x)); CHECK_EQ_I(errno, EDOM); }
TC_CASE(fmod_by_zero) { volatile double x = 1.0, y = 0.0; errno = 0; CHECK_IS_NAN(fmod(x, y)); CHECK_EQ_I(errno, EDOM); }
TC_CASE(fmod_inf_numer) { volatile double y = 2.0; CHECK_IS_NAN(fmod(v_inf, y)); }

int main(void) { tc_begin(); TC_LOAD_PROC(log); TC_LOAD_PROC(log10); TC_LOAD_PROC(sqrt); TC_LOAD_PROC(fmod); init(); TC_RUN(log_negative); TC_RUN(log_zero); TC_RUN(log10_negative); TC_RUN(sqrt_negative); TC_RUN(fmod_by_zero); TC_RUN(fmod_inf_numer); return tc_end(); }
