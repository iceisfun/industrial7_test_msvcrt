/* TC
 * id:    mf_neg_ldexpf_errno_probe
 * area:  math_float
 * funcs:
 * procs: ldexpf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.6.6, 7.12.1; MS docs ldexp (ERANGE on overflow/underflow)
 * note:  PROBE: errno after ldexpf overflow (ERANGE), underflow-to-zero (ERANGE), and for exact-zero / inf inputs (untouched).
 *        If ldexpf is just (float)ldexp((double)x, e) the float overflow/underflow is a conversion and errno may stay 0.
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

TC_DECL_PROC(float, ldexpf, (float, int));

TC_CASE(overflow_erange) { volatile float x = 1.0f; volatile int e = 200; float r; NEED(tc_p_ldexpf); errno = 0; r = tc_p_ldexpf(x, e); CHECK_BITS_F(r, F_INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(underflow_erange) { volatile float x = 1.0f; volatile int e = -200; float r; NEED(tc_p_ldexpf); errno = 0; r = tc_p_ldexpf(x, e); CHECK_BITS_F(r, 0.0f); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(zero_input_errno_unchanged) { volatile float x = 0.0f; volatile int e = 100000; NEED(tc_p_ldexpf); errno = SENT; tc_p_ldexpf(x, e); CHECK_EQ_I(errno, SENT); }
TC_CASE(inf_input_errno_unchanged) { volatile float x = F_INF(); volatile int e = 100; NEED(tc_p_ldexpf); errno = SENT; tc_p_ldexpf(x, e); CHECK_EQ_I(errno, SENT); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(ldexpf);
    TC_RUN(overflow_erange); TC_RUN(underflow_erange); TC_RUN(zero_input_errno_unchanged); TC_RUN(inf_input_errno_unchanged);
    errno = 0; return tc_end();
}
