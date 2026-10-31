/* TC
 * id:    mf_neg_hypotf_inf_zero_errno_probe
 * area:  math_float
 * funcs: _hypotf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _hypot, _hypotf; C11 F.10.4.3
 * note:  probes that _hypotf leaves errno untouched for inf/nan/denormal/zero inputs and returns the C99 values
 *        (hypot(inf,nan)=inf is C99; exact denormal result).
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static float fb(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
#define SENT 1234

TC_CASE(inf_errno_unchanged) { volatile float a = v_inf, b = 3.0f, c = -v_inf, r; errno = SENT; r = _hypotf(a, b); CHECK(r == v_inf); r = _hypotf(b, c); CHECK(r == v_inf); CHECK_EQ_I(errno, SENT); }
TC_CASE(zero_zero) { volatile float a = 0.0f, b = -0.0f; CHECK_BITS_F(_hypotf(a, a), 0.0f); CHECK_BITS_F(_hypotf(b, a), 0.0f); }
TC_CASE(denormal_pair) { volatile float a = fb(1), b = 0.0f, r; errno = SENT; r = _hypotf(a, b); CHECK_BITS_F(r, fb(1)); CHECK_EQ_I(errno, SENT); }
TC_CASE(neg_args_symmetry) { volatile float a = -3.0f, b = -4.0f, c = 4.0f; CHECK_BITS_F(_hypotf(a, b), 5.0f); CHECK_BITS_F(_hypotf(c, a), 5.0f); }
TC_CASE(nan_errno_unchanged) { volatile float a = v_nan, b = 1.0f, r; errno = SENT; r = _hypotf(a, b); CHECK_IS_NAN(r); CHECK_EQ_I(errno, SENT); }

int main(void)
{
    tc_begin(); v_inf = v_one / v_zero; v_nan = fb(0x7fc00000);
    TC_RUN(inf_errno_unchanged); TC_RUN(zero_zero); TC_RUN(denormal_pair); TC_RUN(neg_args_symmetry); TC_RUN(nan_errno_unchanged);
    return tc_end();
}
