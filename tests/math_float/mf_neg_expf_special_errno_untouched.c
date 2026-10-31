/* TC
 * id:    mf_neg_expf_special_errno_untouched
 * area:  math_float
 * funcs:
 * procs: expf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.6.1 / F.10.3.1; MS docs exp
 * note:  crt narrowed to >=120: errno behaviour for inf/nan input in the pre-C99 CRTs is not documented.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <float.h>
#include <limits.h>

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }
#define SENT 0x5a5
#define NEED(p) do { if (!(p)) TC_SKIP("function not exported"); } while (0)

TC_DECL_PROC(float, expf, (float));

TC_CASE(pinf_errno_unchanged) { volatile float x = F_INF(); float r; NEED(tc_p_expf); errno = SENT; r = tc_p_expf(x); CHECK_BITS_F(r, F_INF()); CHECK_EQ_I(errno, SENT); }
TC_CASE(ninf_errno_unchanged) { volatile float x = -F_INF(); float r; NEED(tc_p_expf); errno = SENT; r = tc_p_expf(x); CHECK_BITS_F(r, 0.0f); CHECK_EQ_I(errno, SENT); }
TC_CASE(nan_errno_unchanged) { volatile float x = F_NAN(); float r; NEED(tc_p_expf); errno = SENT; r = tc_p_expf(x); CHECK_IS_NAN(r); CHECK_EQ_I(errno, SENT); }
TC_CASE(iph_count_unchanged)
{
    volatile float a = F_INF(), b = -F_INF(), c = F_NAN();
    NEED(tc_p_expf); CHECK(tc_iph_install()); tc_iph_count = 0;
    tc_p_expf(a); tc_p_expf(b); tc_p_expf(c);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(expf);
    TC_RUN(pinf_errno_unchanged); TC_RUN(ninf_errno_unchanged); TC_RUN(nan_errno_unchanged); TC_RUN(iph_count_unchanged);
    errno = 0; return tc_end();
}
