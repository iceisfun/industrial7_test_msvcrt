/* TC
 * id:    mf_neg_frexpf_inf_nan_exp_probe
 * area:  math_float
 * funcs:
 * procs: frexpf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.6.4 (exp unspecified for inf/nan); MS docs frexp
 * note:  PROBE: the int stored through *exp when frexpf is given inf/nan. The standard leaves it unspecified; UCRT is believed to store 0.
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

TC_CASE(inf_exp_zero) { int e = -99; volatile float x = F_INF(); NEED(tc_p_frexpf); tc_p_frexpf(x, &e); CHECK_EQ_I(e, 0); }
TC_CASE(nan_exp_zero) { int e = -99; volatile float x = F_NAN(); NEED(tc_p_frexpf); tc_p_frexpf(x, &e); CHECK_EQ_I(e, 0); }
TC_CASE(neg_inf_exp_zero) { int e = -99; volatile float x = -F_INF(); NEED(tc_p_frexpf); tc_p_frexpf(x, &e); CHECK_EQ_I(e, 0); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(frexpf);
    TC_RUN(inf_exp_zero); TC_RUN(nan_exp_zero); TC_RUN(neg_inf_exp_zero);
    return tc_end();
}
