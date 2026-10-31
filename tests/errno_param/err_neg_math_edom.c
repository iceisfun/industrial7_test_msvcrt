/* TC
 * id:    err_neg_math_edom
 * area:  errno_param
 * kind:  negative
 * funcs: 
 * procs: sqrt log acos
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sqrt, log, acos return value tables
 */
#include "tc.h"
#include <errno.h>

typedef double (__cdecl *d1_t)(double);
static volatile double v_a, v_b;

static d1_t get1(const char *n) { return (d1_t)tc_crt_proc(n); }

TC_CASE(sqrt_neg) { d1_t f = get1("sqrt"); double r; if (!f) TC_SKIP("sqrt missing"); v_a = -1.0; errno = 0; r = f(v_a); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(log_neg) { d1_t f = get1("log"); double r; if (!f) TC_SKIP("log missing"); v_a = -1.0; errno = 0; r = f(v_a); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(acos_2) { d1_t f = get1("acos"); double r; if (!f) TC_SKIP("acos missing"); v_a = 2.0; errno = 0; r = f(v_a); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }

int main(void) { tc_begin(); TC_RUN(sqrt_neg); TC_RUN(log_neg); TC_RUN(acos_2); return tc_end(); }
