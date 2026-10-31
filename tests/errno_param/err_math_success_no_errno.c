/* TC
 * id:    err_math_success_no_errno
 * area:  errno_param
 * funcs: 
 * procs: sqrt exp
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.5 (errno never cleared by library); MS docs errno
 * note:  PROBE: UCRT math functions do not touch errno on success (neither clear nor set).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

typedef double (__cdecl *d1_t)(double);

TC_CASE(sqrt_zero_errno)
{
    d1_t p = (d1_t)tc_crt_proc("sqrt"); volatile double x = 4.0; double r;
    if (!p) TC_SKIP("sqrt not exported");
    errno = 0; r = p(x);
    CHECK_BITS_D(r, 2.0); CHECK_EQ_I(errno, 0);
}
TC_CASE(sqrt_preserves_errno)
{
    d1_t p = (d1_t)tc_crt_proc("sqrt"); volatile double x = 4.0; double r;
    if (!p) TC_SKIP("sqrt not exported");
    errno = ERANGE; r = p(x);
    CHECK_BITS_D(r, 2.0); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(exp_preserves_errno)
{
    d1_t p = (d1_t)tc_crt_proc("exp"); volatile double x = 0.0; double r;
    if (!p) TC_SKIP("exp not exported");
    errno = EINVAL; r = p(x);
    CHECK_BITS_D(r, 1.0); CHECK_EQ_I(errno, EINVAL);
}
int main(void) { tc_begin(); TC_RUN(sqrt_zero_errno); TC_RUN(sqrt_preserves_errno); TC_RUN(exp_preserves_errno); return tc_end(); }
