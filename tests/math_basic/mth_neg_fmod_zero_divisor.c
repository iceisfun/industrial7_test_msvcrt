/* TC
 * id:    mth_neg_fmod_zero_divisor
 * area:  math_basic
 * funcs:
 * procs: fmod
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fmod (y==0 or x infinite: NaN, errno EDOM); C11 F.10.7.1
 * note:  Observed on an FMA3/AVX2 CPU (ucrtbase dispatches FMA3 paths); non-FMA path unverified.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
/* F: call the DLL export, never the libmingwex static copy (tc_crt_proc = GetProcAddress) */
TC_DECL_PROC(double, fmod, (double, double));
#undef fmod
#define fmod tc_p_fmod

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(one_zero) { double r; errno = 0; r = fmod(D(1.0), D(0.0)); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(negzero) { double r; errno = 0; r = fmod(D(-1.0), D(-0.0)); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(inf_x) { double r; errno = 0; r = fmod(INF(), D(1.0)); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }

int main(void) { tc_begin(); TC_LOAD_PROC(fmod); if (!tc_p_fmod) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(one_zero); TC_RUN(negzero); TC_RUN(inf_x); return tc_end(); }
