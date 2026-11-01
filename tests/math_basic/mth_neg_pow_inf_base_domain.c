/* TC
 * id:    mth_neg_pow_inf_base_domain
 * area:  math_basic
 * funcs:
 * procs: pow
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.4.4, 7.12.1; MS docs pow
 * note:  Observed on an FMA3/AVX2 CPU (ucrtbase dispatches FMA3 paths); non-FMA path unverified.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>
/* F: call the DLL export, never the libmingwex static copy (tc_crt_proc = GetProcAddress) */
TC_DECL_PROC(double, pow, (double, double));
#undef pow
#define pow tc_p_pow

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double NAN_(void) { volatile double i = INF(); return i - i; }
static double D(double x) { volatile double v = x; return v; }
static int NEG(double x) { return tc_dbits(x) >> 63 != 0; }
static void info(const char *w, long long e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(w); tc_write(" = "); tc_write(b); tc_write("\n"); }
#define PI 3.14159265358979323846

TC_CASE(neg_inf_base) { double r; errno = 0; r = pow(-D(INF()), D(3.0)); CHECK_BITS_D(r, -INF()); CHECK_EQ_I(errno, 0);
    r = pow(-D(INF()), D(2.0)); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, 0); r = pow(-D(INF()), D(-3.0)); CHECK_BITS_D(r, -0.0); CHECK_EQ_I(errno, 0); }
TC_CASE(pos_inf_base) { double r; errno = 0; r = pow(D(INF()), D(-1.0)); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); }
TC_CASE(nan_cases) { double r; errno = 0; r = pow(D(NAN_()), D(1.0)); CHECK_IS_NAN(r); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(pow); if (!tc_p_pow) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(neg_inf_base); TC_RUN(pos_inf_base); TC_RUN(nan_cases); return tc_end(); }
