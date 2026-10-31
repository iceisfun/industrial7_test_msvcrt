/* TC
 * id:    mf_neg_frexpf_inf_nan
 * area:  math_float
 * funcs:
 * procs: frexpf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.6.4 / F.10.3.4 (frexp of inf/nan returns it); MS docs frexp
 * note:  crt narrowed to >=120: pre-C99 CRTs (100/110) are not known to return inf/nan unchanged from frexp. -0 sign handling lives in mf_neg_frexpf_negzero_probe.
 *        the value stored in *exp for inf/nan is unspecified and not asserted here (see the probe test).
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

TC_DECL_PROC(float, frexpf, (float, int *));

TC_CASE(inf_returns_inf) { int e = 7; volatile float x = F_INF(); float r; NEED(tc_p_frexpf); r = tc_p_frexpf(x, &e); CHECK_BITS_F(r, F_INF()); }
TC_CASE(ninf_returns_ninf) { int e = 7; volatile float x = -F_INF(); float r; NEED(tc_p_frexpf); r = tc_p_frexpf(x, &e); CHECK_BITS_F(r, -F_INF()); }
TC_CASE(nan_returns_nan) { int e = 7; volatile float x = F_NAN(); float r; NEED(tc_p_frexpf); r = tc_p_frexpf(x, &e); CHECK_IS_NAN(r); }
TC_CASE(zero_exp_is_zero_for_zero)
{
    int e = 7; volatile float z = 0.0f; float r;
    NEED(tc_p_frexpf);
    r = tc_p_frexpf(z, &e); CHECK_BITS_F(r, 0.0f); CHECK_EQ_I(e, 0);
}
TC_CASE(no_iph_called)
{
    int e = 0; volatile float a = F_INF(), b = F_NAN(), c = -F_INF();
    NEED(tc_p_frexpf);
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    tc_p_frexpf(a, &e); tc_p_frexpf(b, &e); tc_p_frexpf(c, &e);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(frexpf);
    TC_RUN(inf_returns_inf); TC_RUN(ninf_returns_ninf); TC_RUN(nan_returns_nan); TC_RUN(zero_exp_is_zero_for_zero); TC_RUN(no_iph_called);
    return tc_end();
}
