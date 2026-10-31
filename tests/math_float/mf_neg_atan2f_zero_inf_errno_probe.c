/* TC
 * id:    mf_neg_atan2f_zero_inf_errno_probe
 * area:  math_float
 * funcs: atan2f
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs atan2, atan2f; C11 F.10.1.4
 * note:  probes only errno of atan2f(+-0,+-0) (ucrt believed to leave errno unchanged, older CRTs set EDOM),
 *        atan2f(inf,inf) and NaN operands. Values themselves are covered by documented tests.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static float fb(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
#define SENT 1234

TC_CASE(pz_pz_errno) { volatile float y = 0.0f, x = 0.0f, r; errno = SENT; r = atan2f(y, x); (void)r; CHECK_EQ_I(errno, SENT); }
TC_CASE(nz_pz_errno) { volatile float y = -0.0f, x = 0.0f, r; errno = SENT; r = atan2f(y, x); (void)r; CHECK_EQ_I(errno, SENT); }
TC_CASE(pz_nz_errno) { volatile float y = 0.0f, x = -0.0f, r; errno = SENT; r = atan2f(y, x); (void)r; CHECK_EQ_I(errno, SENT); }
TC_CASE(inf_inf_errno_unchanged) { volatile float y = v_inf, x = v_inf, r; errno = SENT; r = atan2f(y, x); (void)r; CHECK_EQ_I(errno, SENT); }
TC_CASE(nan_errno_unchanged) { volatile float y = v_nan, x = 1.0f, r; errno = SENT; r = atan2f(y, x); CHECK_IS_NAN(r); CHECK_EQ_I(errno, SENT); }

int main(void)
{
    tc_begin(); v_inf = v_one / v_zero; v_nan = fb(0x7fc00000);
    TC_RUN(pz_pz_errno); TC_RUN(nz_pz_errno); TC_RUN(pz_nz_errno); TC_RUN(inf_inf_errno_unchanged); TC_RUN(nan_errno_unchanged);
    return tc_end();
}
