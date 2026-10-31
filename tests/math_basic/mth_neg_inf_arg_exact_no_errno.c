/* TC
 * id:    mth_neg_inf_arg_exact_no_errno
 * area:  math_basic
 * funcs: exp log sqrt sinh cosh pow atan atan2 tanh fmod
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.3, F.10.4.4, 7.12.1; MS docs exp, pow
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double NAN_(void) { volatile double i = INF(); return i - i; }
static double D(double x) { volatile double v = x; return v; }
static int NEG(double x) { return tc_dbits(x) >> 63 != 0; }
static void info(const char *w, long long e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(w); tc_write(" = "); tc_write(b); tc_write("\n"); }
#define PI 3.14159265358979323846

#define R(var, call) do { errno = 0; var = (call); } while (0)
TC_CASE(exp_inf) { double r; R(r, exp(-D(INF()))); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); R(r, exp(D(INF()))); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, 0); }
TC_CASE(log_sqrt_inf) { double r; R(r, log(D(INF()))); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, 0); R(r, sqrt(D(INF()))); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, 0); }
TC_CASE(sinh_cosh_inf) { double r;
    R(r, sinh(D(INF()))); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, 0);
    R(r, sinh(-D(INF()))); CHECK_BITS_D(r, -INF()); CHECK_EQ_I(errno, 0);
    R(r, cosh(D(INF()))); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, 0);
    R(r, cosh(-D(INF()))); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, 0); }
TC_CASE(pow_inf_exp) { double r; R(r, pow(D(2.0), D(INF()))); CHECK_BITS_D(r, INF()); CHECK_EQ_I(errno, 0); R(r, pow(D(2.0), -D(INF()))); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); }
TC_CASE(atan_inf) { double r; R(r, atan(D(INF()))); CHECK_NEAR(r, PI / 2, 1e-15); CHECK_EQ_I(errno, 0); R(r, atan(-D(INF()))); CHECK_NEAR(r, -PI / 2, 1e-15); CHECK_EQ_I(errno, 0);
    R(r, atan2(D(INF()), D(INF()))); CHECK_NEAR(r, PI / 4, 1e-15); CHECK_EQ_I(errno, 0); }
TC_CASE(tanh_inf) { double r; R(r, tanh(D(INF()))); CHECK_BITS_D(r, 1.0); CHECK_EQ_I(errno, 0); R(r, tanh(-D(INF()))); CHECK_BITS_D(r, -1.0); CHECK_EQ_I(errno, 0); }
TC_CASE(fmod_finite_by_inf) { double r; R(r, fmod(D(1.0), D(INF()))); CHECK_BITS_D(r, 1.0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(exp_inf); TC_RUN(log_sqrt_inf); TC_RUN(sinh_cosh_inf); TC_RUN(pow_inf_exp); TC_RUN(atan_inf); TC_RUN(tanh_inf); TC_RUN(fmod_finite_by_inf); return tc_end(); }
