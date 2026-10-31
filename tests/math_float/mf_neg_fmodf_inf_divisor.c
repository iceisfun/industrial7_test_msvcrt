/* TC
 * id:    mf_neg_fmodf_inf_divisor
 * area:  math_float
 * funcs: fmodf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.7.1; MS docs fmod, fmodf
 * note:  ucrtbase only: pre-UCRT x64 fmodf(x, +-inf) handling not known to follow C99 Annex F.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>

static volatile float v_zero = 0.0f, v_one = 1.0f, v_inf, v_nan;
static float fb(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
#define SENT 1234

TC_CASE(x_mod_pinf_is_x) { volatile float x = 5.0f, y = v_inf; CHECK_BITS_F(fmodf(x, y), 5.0f); }
TC_CASE(x_mod_ninf_is_x) { volatile float x = 5.0f, y = -v_inf; CHECK_BITS_F(fmodf(x, y), 5.0f); }
TC_CASE(neg_x_mod_inf_is_negx) { volatile float x = -5.0f, y = v_inf; CHECK_BITS_F(fmodf(x, y), -5.0f); }
TC_CASE(zero_mod_nonzero_zero) { volatile float p = 0.0f, n = -0.0f, y = 3.0f; CHECK_BITS_F(fmodf(p, y), 0.0f); CHECK_BITS_F(fmodf(n, y), -0.0f); }
TC_CASE(zero_mod_zero_nan) { volatile float p = 0.0f, q = 0.0f; CHECK_IS_NAN(fmodf(p, q)); }
TC_CASE(valid_errno_unchanged)
{
    volatile float a = 5.0f, i = v_inf, b = 3.0f, r;
    errno = SENT; r = fmodf(a, i); r = fmodf(a, b); r = fmodf(-a, i); (void)r;
    CHECK_EQ_I(errno, SENT);
}
TC_CASE(tiny_mod_huge_is_tiny) { volatile float x = 1e-30f, y = 1e30f; CHECK_BITS_F(fmodf(x, y), 1e-30f); }

int main(void)
{
    tc_begin(); v_inf = v_one / v_zero;
    TC_RUN(x_mod_pinf_is_x); TC_RUN(x_mod_ninf_is_x); TC_RUN(neg_x_mod_inf_is_negx); TC_RUN(zero_mod_nonzero_zero);
    TC_RUN(zero_mod_zero_nan); TC_RUN(valid_errno_unchanged); TC_RUN(tiny_mod_huge_is_tiny);
    return tc_end();
}
