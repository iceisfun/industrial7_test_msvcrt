/* TC
 * id:    mf_neg_sqrtf_negzero_inf
 * area:  math_float
 * funcs:
 * procs: sqrtf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 F.10.4.5 (sqrt(-0) = -0); MS docs sqrt (negative input: EDOM, indefinite NaN)
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

TC_DECL_PROC(float, sqrtf, (float));
static float from_bits(unsigned int u) { union { unsigned int u; float f; } x; x.u = u; return x.f; }
static volatile unsigned int v_negd = 0x80000001u, v_negnan = 0xffc00001u;

TC_CASE(negzero_stays_negzero) { volatile float x = -0.0f; NEED(tc_p_sqrtf); CHECK_BITS_F(tc_p_sqrtf(x), -0.0f); }
TC_CASE(negzero_no_edom) { volatile float x = -0.0f; NEED(tc_p_sqrtf); errno = SENT; tc_p_sqrtf(x); CHECK_EQ_I(errno, SENT); }
TC_CASE(pinf_ok) { volatile float x = F_INF(); NEED(tc_p_sqrtf); errno = SENT; CHECK_BITS_F(tc_p_sqrtf(x), F_INF()); CHECK_EQ_I(errno, SENT); }
TC_CASE(neg_min_denormal_nan_edom) { volatile float x = from_bits(v_negd); float r; NEED(tc_p_sqrtf); errno = 0; r = tc_p_sqrtf(x); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(negative_nan_in_nan_out) { volatile float x = from_bits(v_negnan); NEED(tc_p_sqrtf); CHECK_IS_NAN(tc_p_sqrtf(x)); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(sqrtf);
    TC_RUN(negzero_stays_negzero); TC_RUN(negzero_no_edom); TC_RUN(pinf_ok); TC_RUN(neg_min_denormal_nan_edom); TC_RUN(negative_nan_in_nan_out);
    errno = 0; return tc_end();
}
