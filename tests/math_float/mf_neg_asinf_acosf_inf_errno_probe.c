/* TC
 * id:    mf_neg_asinf_acosf_inf_errno_probe
 * area:  math_float
 * funcs:
 * procs: asinf acosf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.4.1/7.12.4.2 (domain error); MS docs asin, acos (EDOM for |x|>1)
 * note:  PROBE: asinf/acosf(+-inf) set EDOM (inf is outside [-1,1]); a quiet NaN input leaves errno untouched.
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

TC_DECL_PROC(float, asinf, (float));
TC_DECL_PROC(float, acosf, (float));

TC_CASE(asinf_pinf_edom) { volatile float x = F_INF(); float r; NEED(tc_p_asinf); errno = 0; r = tc_p_asinf(x); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(acosf_ninf_edom) { volatile float x = -F_INF(); float r; NEED(tc_p_acosf); errno = 0; r = tc_p_acosf(x); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); }
TC_CASE(asinf_nan_errno_unchanged) { volatile float x = F_NAN(); float r; NEED(tc_p_asinf); errno = SENT; r = tc_p_asinf(x); CHECK_IS_NAN(r); CHECK_EQ_I(errno, SENT); }
TC_CASE(acosf_nan_errno_unchanged) { volatile float x = F_NAN(); float r; NEED(tc_p_acosf); errno = SENT; r = tc_p_acosf(x); CHECK_IS_NAN(r); CHECK_EQ_I(errno, SENT); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(asinf); TC_LOAD_PROC(acosf);
    TC_RUN(asinf_pinf_edom); TC_RUN(acosf_ninf_edom); TC_RUN(asinf_nan_errno_unchanged); TC_RUN(acosf_nan_errno_unchanged);
    errno = 0; return tc_end();
}
