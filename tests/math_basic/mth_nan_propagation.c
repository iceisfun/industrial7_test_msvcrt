/* TC
 * id:    mth_nan_propagation
 * area:  math_basic
 * funcs: sin cos tan asin acos atan atan2 sinh cosh tanh exp log log10 sqrt floor ceil fmod ldexp
 * procs: fabs
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10; MS docs math functions (NaN in, NaN out)
 */
#include "tc.h"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static double qnan(void) { return mk(0x7FF8000000000000ULL); }
static double pinf(void) { return mk(0x7FF0000000000000ULL); }

static double (__cdecl *p_fabs)(double);
#define U(f) CHECK_IS_NAN(f(x))
TC_CASE(unary_trig) { volatile double x = qnan(); U(sin); U(cos); U(tan); U(asin); U(acos); U(atan); }
TC_CASE(unary_other) { volatile double x = qnan(); U(sinh); U(cosh); U(tanh); U(exp); U(log); U(log10); U(sqrt); CHECK(p_fabs != NULL); if (p_fabs) CHECK_IS_NAN(p_fabs(x)); U(floor); U(ceil); }
TC_CASE(atan2_) { volatile double n = qnan(), o = 1.0; CHECK_IS_NAN(atan2(n, o)); CHECK_IS_NAN(atan2(o, n)); }
TC_CASE(fmod_) { volatile double n = qnan(), o = 1.0; CHECK_IS_NAN(fmod(n, o)); CHECK_IS_NAN(fmod(o, n)); }
TC_CASE(ldexp_) { volatile double n = qnan(); volatile int e = 1; CHECK_IS_NAN(ldexp(n, e)); }
int main(void) { tc_begin(); errno = 0; p_fabs = (double (__cdecl *)(double))tc_crt_proc("fabs"); TC_RUN(unary_trig); TC_RUN(unary_other); TC_RUN(atan2_); TC_RUN(fmod_); TC_RUN(ldexp_); return tc_end(); }
