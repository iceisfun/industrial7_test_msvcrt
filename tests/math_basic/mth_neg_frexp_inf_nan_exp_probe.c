/* TC
 * id:    mth_neg_frexp_inf_nan_exp_probe
 * area:  math_basic
 * funcs:
 * procs: frexp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.6.4 (exponent unspecified for inf/NaN)
 * note:  Probe: frexp(+-inf)/frexp(nan) return arg unchanged; stored exponent and errno are printed only, except exponent 0 asserted in separate case.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

TC_DECL_PROC(double, frexp, (double, int *));
#define frexp tc_p_frexp

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double NAN_(void) { volatile double i = INF(); return i - i; }
static double D(double x) { volatile double v = x; return v; }
static int NEG(double x) { return tc_dbits(x) >> 63 != 0; }
static void info(const char *w, long long e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(w); tc_write(" = "); tc_write(b); tc_write("\n"); }
#define PI 3.14159265358979323846

TC_CASE(inf) { int e = 77; double r = frexp(D(INF()), &e); CHECK_BITS_D(r, INF()); r = frexp(-D(INF()), &e); CHECK_BITS_D(r, -INF()); }
TC_CASE(nan) { int e = 77; double r = frexp(D(NAN_()), &e); CHECK_IS_NAN(r); }
TC_CASE(exp_zero) { int e = 77; frexp(D(INF()), &e); info("frexp(inf) exp", e); e = 77; frexp(D(NAN_()), &e); info("frexp(nan) exp", e); CHECK_EQ_I(e, 0); }
TC_CASE(errno_info) { int e; errno = 0; frexp(D(INF()), &e); info("frexp(inf) errno", errno); errno = 0; frexp(D(NAN_()), &e); info("frexp(nan) errno", errno); }

int main(void) { tc_begin(); TC_LOAD_PROC(frexp); TC_RUN(inf); TC_RUN(nan); TC_RUN(exp_zero); TC_RUN(errno_info); return tc_end(); }
