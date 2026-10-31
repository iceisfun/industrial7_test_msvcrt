/* TC
 * id:    fpe_neg_statusfp_clean_after_clear
 * area:  fpenv
 * kind:  negative
 * funcs: _clearfp _statusfp _controlfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _clearfp, _statusfp; C11 F.3 (== on quiet NaN does not raise invalid), exact arithmetic raises nothing
 * note:  Status flags must stay clear when no exception condition occurred.
 */
#include "tc.h"
#include <float.h>

#define SW_DENORMAL 0x80000
static volatile double v_zero = 0.0, v_two = 2.0, v_three = 3.0, v_nan, v_one = 1.0;
static volatile double sink;

TC_CASE(clear_then_zero)
{
    _controlfp(_MCW_EM, _MCW_EM);
    sink = v_one / v_zero;            /* raise something first */
    CHECK(_statusfp() != 0);
    _clearfp();
    CHECK_EQ_U(_statusfp(), 0);
}
TC_CASE(clearfp_returns_old)
{
    unsigned old;
    _controlfp(_MCW_EM, _MCW_EM); _clearfp();
    sink = v_one / v_zero;
    old = _clearfp();
    CHECK((old & 0x08) != 0);         /* zero-divide flag was reported by _clearfp */
    CHECK_EQ_U(_statusfp(), 0);
}
TC_CASE(exact_ops_no_flags)
{
    _controlfp(_MCW_EM, _MCW_EM); _clearfp();
    sink = v_two * v_three;
    sink = v_two + v_three;
    sink = v_three - v_two;
    CHECK_EQ_U(_statusfp(), 0);
}
TC_CASE(quiet_nan_equality_no_invalid)
{
    int r;
    _controlfp(_MCW_EM, _MCW_EM);
    v_nan = v_zero / v_zero; _clearfp();
    r = (v_nan == v_nan);
    CHECK_EQ_I(r, 0);
    r = (v_nan == v_two);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(_statusfp() & 0x10, 0);
    _clearfp();
}
TC_CASE(no_denormal_flag)
{
    _controlfp(_MCW_EM, _MCW_EM); _clearfp();
    sink = v_two * v_three;
    CHECK_EQ_U(_statusfp() & SW_DENORMAL, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(clear_then_zero); TC_RUN(clearfp_returns_old); TC_RUN(exact_ops_no_flags);
    TC_RUN(quiet_nan_equality_no_invalid); TC_RUN(no_denormal_flag);
    return tc_end();
}
