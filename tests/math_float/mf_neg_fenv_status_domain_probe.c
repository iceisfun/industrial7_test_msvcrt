/* TC
 * id:    mf_neg_fenv_status_domain_probe
 * area:  math_float
 * funcs:
 * procs: _clearfp _statusfp sqrtf logf expf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _clearfp, _statusfp (_SW_INVALID 0x10, _SW_ZERODIVIDE 0x08, _SW_OVERFLOW 0x04, _SW_INEXACT 0x01)
 * note:  PROBE: whether sqrtf(-1), logf(0), expf(1000) leave the matching sticky flag in the FP status word
 *        as reported by _statusfp. A CRT may compute these in software without raising hardware flags.
 *        Each case is independent; the only documented assertion is the control case (flags are cleared).
 */
#include "tc.h"

typedef float (__cdecl *f1_t)(float);
typedef unsigned int (__cdecl *fp_t)(void);
static f1_t p_sqrtf, p_logf, p_expf;
static fp_t p_clearfp, p_statusfp;
#define SW_INEXACT 0x01
#define SW_OVERFLOW 0x04
#define SW_ZERODIVIDE 0x08
#define SW_INVALID 0x10
static volatile float v_m1 = -1.0f, v_zero = 0.0f, v_big = 1000.0f, v_four = 4.0f;
static volatile float sink;

TC_CASE(control_clear) { p_clearfp(); CHECK_EQ_U(p_statusfp() & (SW_INVALID | SW_ZERODIVIDE | SW_OVERFLOW), 0); }
TC_CASE(sqrtf_neg_invalid) { p_clearfp(); sink = p_sqrtf(v_m1); CHECK((p_statusfp() & SW_INVALID) != 0); p_clearfp(); }
TC_CASE(sqrtf_ok_no_invalid) { p_clearfp(); sink = p_sqrtf(v_four); CHECK((p_statusfp() & SW_INVALID) == 0); p_clearfp(); }
TC_CASE(logf_zero_divide) { p_clearfp(); sink = p_logf(v_zero); CHECK((p_statusfp() & SW_ZERODIVIDE) != 0); p_clearfp(); }
TC_CASE(expf_overflow) { p_clearfp(); sink = p_expf(v_big); CHECK((p_statusfp() & SW_OVERFLOW) != 0); p_clearfp(); }
TC_CASE(expf_overflow_inexact) { p_clearfp(); sink = p_expf(v_big); CHECK((p_statusfp() & SW_INEXACT) != 0); p_clearfp(); }

int main(void)
{
    tc_begin();
    p_sqrtf = (f1_t)tc_crt_proc("sqrtf"); p_logf = (f1_t)tc_crt_proc("logf"); p_expf = (f1_t)tc_crt_proc("expf");
    p_clearfp = (fp_t)tc_crt_proc("_clearfp"); p_statusfp = (fp_t)tc_crt_proc("_statusfp");
    if (!p_sqrtf || !p_logf || !p_expf || !p_clearfp || !p_statusfp) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; }
    TC_RUN(control_clear); TC_RUN(sqrtf_neg_invalid); TC_RUN(sqrtf_ok_no_invalid); TC_RUN(logf_zero_divide);
    TC_RUN(expf_overflow); TC_RUN(expf_overflow_inexact);
    return tc_end();
}
