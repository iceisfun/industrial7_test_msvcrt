/* TC
 * id:    mth_neg_pow_underflow_probe
 * area:  math_basic
 * funcs:
 * procs: pow
 * crt:   msvcrt msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.12.1 (underflow: ERANGE implementation-defined)
 * note:  Calls go to the real ucrtbase export via GetProcAddress (a bare call in a mingw exe lands in libmingwex static code, not the DLL). OBSERVED Win10 22H2: pow(10,-400)==+0 and errno stays 0 (same _UNDERFLOW rule; FMA3-vs-SSE2 path selection is CPU dependent, errno result expected identical).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
TC_DECL_PROC(double, pow, (double, double));
#undef pow
#define pow tc_p_pow

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(value) { double r = pow(D(10.0), D(-400.0)); CHECK_BITS_D(r, 0.0); }
TC_CASE(errno_range) { double r; errno = 0; r = pow(D(10.0), D(-400.0)); (void)r; CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(pow); if (!tc_p_pow) { tc_write("TC FAIL " TC_TEST_ID "::proc pow not exported\n"); return 1; } TC_RUN(value); TC_RUN(errno_range); return tc_end(); }
