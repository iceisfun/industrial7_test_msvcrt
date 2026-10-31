/* TC
 * id:    err_neg_math_underflow_probe
 * area:  errno_param
 * kind:  negative
 * funcs: 
 * procs: exp pow
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs exp (underflow)
 * note:  PROBE: errno after exp(-1000) / pow(10,-400) underflow to 0 is ERANGE on UCRT (assumed).
 */
#include "tc.h"
#include <errno.h>

typedef double (__cdecl *d1_t)(double);
typedef double (__cdecl *d2_t)(double, double);
static volatile double v_a, v_b;

TC_CASE(exp_underflow_errno) { d1_t f = (d1_t)tc_crt_proc("exp"); if (!f) TC_SKIP("exp missing"); v_a = -1000.0; errno = 0; f(v_a); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(pow_underflow_errno) { d2_t f = (d2_t)tc_crt_proc("pow"); if (!f) TC_SKIP("pow missing"); v_a = 10.0; v_b = -400.0; errno = 0; f(v_a, v_b); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(exp_underflow_errno); TC_RUN(pow_underflow_errno); return tc_end(); }
