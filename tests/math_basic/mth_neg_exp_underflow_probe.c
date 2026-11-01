/* TC
 * id:    mth_neg_exp_underflow_probe
 * area:  math_basic
 * funcs:
 * procs: exp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.12.1 (underflow: ERANGE implementation-defined)
 * note:  Calls go to the real ucrtbase export via GetProcAddress (a bare call in a mingw exe lands in libmingwex static code, not the DLL). OBSERVED Win10 22H2: exp(-1000)==+0 and errno stays 0 (ucrtbase _set_errno_from_matherr sets nothing for _UNDERFLOW; C11 7.12.1 makes underflow ERANGE implementation-defined). CPU note: exp/pow pick FMA3 vs SSE2 path (__use_fma3_lib; reference _get_FMA3_enable=1); errno result expected identical on both.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
TC_DECL_PROC(double, exp, (double));
#undef exp
#define exp tc_p_exp

static volatile double v_zero = 0.0, v_one = 1.0;
static double INF(void) { return v_one / v_zero; }
static double D(double x) { volatile double v = x; return v; }

TC_CASE(value) { double r = exp(D(-1000.0)); CHECK_BITS_D(r, 0.0); }
TC_CASE(errno_range) { double r; errno = 0; r = exp(D(-1000.0)); (void)r; CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(exp); if (!tc_p_exp) { tc_write("TC FAIL " TC_TEST_ID "::proc exp not exported\n"); return 1; } TC_RUN(value); TC_RUN(errno_range); return tc_end(); }
