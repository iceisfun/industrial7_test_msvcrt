/* TC
 * id:    err_neg_math_underflow_probe
 * area:  errno_param
 * kind:  negative
 * funcs: 
 * procs: exp pow
 * crt:   >=140
 * tier:  p2
 * conf:  observed
 * ref:   MS docs exp (underflow)
 * note:  Observed on Win10 22H2 19045 ucrtbase: exp(-1000) and pow(10,-400) underflow to +0.0 and errno stays 0 (no ERANGE on
 *        underflow). C11 7.12.1p2-3 leaves errno on underflow implementation-defined, so this is a documented-by-observation
 *        UCRT choice, not an ISO deviation. CPU dependence: ucrtbase dispatches exp/pow to FMA3 variants at run time (__use_fma3_lib);
 *        errno==0 was observed only on the reference CPU (FMA3 path) and is [unverified] on non-FMA3 CPUs.
 */
#include "tc.h"
#include <errno.h>

typedef double (__cdecl *d1_t)(double);
typedef double (__cdecl *d2_t)(double, double);
static volatile double v_a, v_b;

TC_CASE(exp_underflow_errno) { d1_t f = (d1_t)tc_crt_proc("exp"); if (!f) TC_SKIP("exp missing"); v_a = -1000.0; errno = 0; double r = f(v_a); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); }
TC_CASE(pow_underflow_errno) { d2_t f = (d2_t)tc_crt_proc("pow"); if (!f) TC_SKIP("pow missing"); v_a = 10.0; v_b = -400.0; errno = 0; double r = f(v_a, v_b); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(exp_underflow_errno); TC_RUN(pow_underflow_errno); return tc_end(); }
