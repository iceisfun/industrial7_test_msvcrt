/* TC
 * id:    err_neg_math_range
 * area:  errno_param
 * kind:  negative
 * funcs: 
 * procs: exp pow
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs exp, pow return value tables
 */
#include "tc.h"
#include <errno.h>
#include <math.h>

typedef double (__cdecl *d1_t)(double);
typedef double (__cdecl *d2_t)(double, double);
static volatile double v_a, v_b;

TC_CASE(exp_overflow) { d1_t f = (d1_t)tc_crt_proc("exp"); double r; if (!f) TC_SKIP("exp missing"); v_a = 1000.0; errno = 0; r = f(v_a); CHECK(r == HUGE_VAL); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(pow_overflow) { d2_t f = (d2_t)tc_crt_proc("pow"); double r; if (!f) TC_SKIP("pow missing"); v_a = 10.0; v_b = 400.0; errno = 0; r = f(v_a, v_b); CHECK(r == HUGE_VAL); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(exp_underflow_value) { d1_t f = (d1_t)tc_crt_proc("exp"); double r; if (!f) TC_SKIP("exp missing"); v_a = -1000.0; r = f(v_a); CHECK_BITS_D(r, 0.0); }

int main(void) { tc_begin(); TC_RUN(exp_overflow); TC_RUN(pow_overflow); TC_RUN(exp_underflow_value); return tc_end(); }
