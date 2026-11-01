/* TC
 * id:    mth_neg_inf_arg_exact_no_errno
 * area:  math_basic
 * funcs:
 * procs: exp log sqrt sinh cosh atan tanh pow atan2 fmod
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.3, F.10.4.4, 7.12.1; MS docs exp, pow
 * note:  Observed on an FMA3/AVX2 CPU (ucrtbase dispatches FMA3 paths); non-FMA path unverified.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>
/* F: call the DLL export, never the libmingwex static copy (tc_crt_proc = GetProcAddress) */
TC_DECL_PROC(double, exp, (double));
#undef exp
#define exp tc_p_exp
TC_DECL_PROC(double, log, (double));
#undef log
#define log tc_p_log
TC_DECL_PROC(double, sqrt, (double));
#undef sqrt
#define sqrt tc_p_sqrt
TC_DECL_PROC(double, sinh, (double));
#undef sinh
#define sinh tc_p_sinh
TC_DECL_PROC(double, cosh, (double));
#undef cosh
#define cosh tc_p_cosh
TC_DECL_PROC(double, atan, (double));
#undef atan
#define atan tc_p_atan
TC_DECL_PROC(double, tanh, (double));
#undef tanh
#define tanh tc_p_tanh
TC_DECL_PROC(double, pow, (double, double));
#undef pow
#define pow tc_p_pow
TC_DECL_PROC(double, atan2, (double, double));
#undef atan2
#define atan2 tc_p_atan2
TC_DECL_PROC(double, fmod, (double, double));
#undef fmod
#define fmod tc_p_fmod

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

int main(void) { tc_begin(); TC_LOAD_PROC(exp); TC_LOAD_PROC(log); TC_LOAD_PROC(sqrt); TC_LOAD_PROC(sinh); TC_LOAD_PROC(cosh); TC_LOAD_PROC(atan); TC_LOAD_PROC(tanh); TC_LOAD_PROC(pow); TC_LOAD_PROC(atan2); TC_LOAD_PROC(fmod); if (!tc_p_exp || !tc_p_log || !tc_p_sqrt || !tc_p_sinh || !tc_p_cosh || !tc_p_atan || !tc_p_tanh || !tc_p_pow || !tc_p_atan2 || !tc_p_fmod) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(exp_inf); TC_RUN(log_sqrt_inf); TC_RUN(sinh_cosh_inf); TC_RUN(pow_inf_exp); TC_RUN(atan_inf); TC_RUN(tanh_inf); TC_RUN(fmod_finite_by_inf); return tc_end(); }
