/* TC
 * id:    m99_fe_neg_status_domain_probe
 * area:  math_c99
 * funcs: acosh log2 exp2 tgamma
 * procs: _statusfp _clearfp
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10 / 7.12.1; MS docs _statusfp
 * note:  PROBE: whether the C99 functions raise the IEEE exception flags (visible through _statusfp)
 *        on domain / range errors. C Annex F says they should; UCRT math_errhandling advertises
 *        MATH_ERREXCEPT, msvcr120 behaviour is uncertain. Each case is an independent guess.
 */
#include "tc.h"
#include <math.h>
#include <float.h>

#define TC_SW_INVALID 0x10u
#define TC_SW_ZERODIVIDE 0x08u
#define TC_SW_OVERFLOW 0x04u
#define TC_SW_UNDERFLOW 0x02u
typedef unsigned int (__cdecl *fpfn_t)(void);
static fpfn_t p_status, p_clear;
static volatile double v_a;

TC_CASE(acosh_below_one_invalid)
{
    if (!p_status || !p_clear) TC_SKIP("no _statusfp");
    v_a = 0.5; p_clear();
    { volatile double r = acosh(v_a); CHECK(r != r); }
    CHECK((p_status() & TC_SW_INVALID) != 0);
}
TC_CASE(log2_zero_divzero)
{
    if (!p_status || !p_clear) TC_SKIP("no _statusfp");
    v_a = 0.0; p_clear();
    { volatile double r = log2(v_a); CHECK(r == -HUGE_VAL); }
    CHECK((p_status() & TC_SW_ZERODIVIDE) != 0);
}
TC_CASE(exp2_overflow)
{
    if (!p_status || !p_clear) TC_SKIP("no _statusfp");
    v_a = 1024.0; p_clear();
    { volatile double r = exp2(v_a); CHECK(r == HUGE_VAL); }
    CHECK((p_status() & TC_SW_OVERFLOW) != 0);
}
TC_CASE(exp2_underflow)
{
    if (!p_status || !p_clear) TC_SKIP("no _statusfp");
    v_a = -1100.0; p_clear();
    { volatile double r = exp2(v_a); CHECK(r == 0.0); }
    CHECK((p_status() & TC_SW_UNDERFLOW) != 0);
}
TC_CASE(tgamma_neg_int_invalid)
{
    if (!p_status || !p_clear) TC_SKIP("no _statusfp");
    v_a = -1.0; p_clear();
    { volatile double r = tgamma(v_a); CHECK(r != r); }
    CHECK((p_status() & TC_SW_INVALID) != 0);
}
int main(void)
{
    tc_begin();
    p_status = (fpfn_t)tc_crt_proc("_statusfp"); p_clear = (fpfn_t)tc_crt_proc("_clearfp");
    TC_RUN(acosh_below_one_invalid); TC_RUN(log2_zero_divzero); TC_RUN(exp2_overflow);
    TC_RUN(exp2_underflow); TC_RUN(tgamma_neg_int_invalid);
    if (p_clear) p_clear();
    return tc_end();
}
