/* TC
 * id:    mf_neg_frexpf_negzero_probe
 * area:  math_float
 * funcs:
 * procs: frexpf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.3.4 (frexp(+-0) returns +-0, *exp = 0)
 * note:  PROBE: frexpf(-0.0f) preserves the sign bit and stores exp 0. Older CRTs may return +0; only UCRT is probed.
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

TC_CASE(negzero_sign_kept) { int e = 7; volatile float nz = -0.0f; float r; NEED(tc_p_frexpf); r = tc_p_frexpf(nz, &e); CHECK_BITS_F(r, -0.0f); CHECK_EQ_I(e, 0); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(frexpf);
    TC_RUN(negzero_sign_kept);
    return tc_end();
}
