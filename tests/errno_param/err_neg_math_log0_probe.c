/* TC
 * id:    err_neg_math_log0_probe
 * area:  errno_param
 * kind:  negative
 * funcs: 
 * procs: log pow
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   C11 F.10.3.11 (pole error), 7.12.1; MS docs log, pow
 * note:  PROBE: errno value (expected ERANGE; EDOM is the alternative) for pole errors; result must be +-inf.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

typedef double (__cdecl *d1_t)(double);
typedef double (__cdecl *d2_t)(double, double);

TC_CASE(log_zero)
{
    d1_t p_log = (d1_t)tc_crt_proc("log"); volatile double z = 0.0; double r;
    if (!p_log) TC_SKIP("log not exported");
    errno = 0; r = p_log(z);
    CHECK_BITS_D(r, -HUGE_VAL);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(pow_zero_negexp)
{
    d2_t p_pow = (d2_t)tc_crt_proc("pow"); volatile double z = 0.0, e = -1.0; double r;
    if (!p_pow) TC_SKIP("pow not exported");
    errno = 0; r = p_pow(z, e);
    CHECK_BITS_D(r, HUGE_VAL);
    CHECK_EQ_I(errno, ERANGE);
}
int main(void) { tc_begin(); TC_RUN(log_zero); TC_RUN(pow_zero_negexp); return tc_end(); }
