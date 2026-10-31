/* TC
 * id:    mf_neg_coshf_sinhf_errno_probe
 * area:  math_float
 * funcs:
 * procs: coshf sinhf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.5.4, 7.12.5.5; MS docs cosh, sinh
 * note:  PROBE: errno after coshf overflow from a finite argument (ERANGE) and untouched errno for inf/nan inputs.
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

TC_DECL_PROC(float, coshf, (float));
TC_DECL_PROC(float, sinhf, (float));

TC_CASE(coshf_neg100_erange) { volatile float x = -100.0f; float r; NEED(tc_p_coshf); errno = 0; r = tc_p_coshf(x); CHECK_BITS_F(r, F_INF()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(coshf_inf_errno_unchanged) { volatile float x = F_INF(); float r; NEED(tc_p_coshf); errno = SENT; r = tc_p_coshf(x); CHECK_BITS_F(r, F_INF()); CHECK_EQ_I(errno, SENT); }
TC_CASE(sinhf_ninf_errno_unchanged) { volatile float x = -F_INF(); float r; NEED(tc_p_sinhf); errno = SENT; r = tc_p_sinhf(x); CHECK_BITS_F(r, -F_INF()); CHECK_EQ_I(errno, SENT); }
TC_CASE(sinhf_nan_errno_unchanged) { volatile float x = F_NAN(); float r; NEED(tc_p_sinhf); errno = SENT; r = tc_p_sinhf(x); CHECK_IS_NAN(r); CHECK_EQ_I(errno, SENT); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(coshf); TC_LOAD_PROC(sinhf);
    TC_RUN(coshf_neg100_erange); TC_RUN(coshf_inf_errno_unchanged); TC_RUN(sinhf_ninf_errno_unchanged); TC_RUN(sinhf_nan_errno_unchanged);
    errno = 0; return tc_end();
}
