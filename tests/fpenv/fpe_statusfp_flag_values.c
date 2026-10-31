/* TC
 * id:    fpe_statusfp_flag_values
 * area:  fpenv
 * funcs: _statusfp _controlfp
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _controlfp, float.h
 * note:  _MCW_EM is 0x0008001F in the MS float.h (includes _EM_DENORMAL), not 0x1F.
 */
#include "tc.h"
#include <float.h>
#ifndef _SW_INEXACT /* mingw float.h lacks the _SW_* aliases of the MS header; values per MS float.h */
#define _SW_INEXACT 0x1
#define _SW_UNDERFLOW 0x2
#define _SW_OVERFLOW 0x4
#define _SW_ZERODIVIDE 0x8
#define _SW_INVALID 0x10
#define _SW_DENORMAL 0x80000
#endif

TC_CASE(sw_values)
{
    CHECK_EQ_U(_SW_INEXACT, 0x1); CHECK_EQ_U(_SW_UNDERFLOW, 0x2); CHECK_EQ_U(_SW_OVERFLOW, 0x4);
    CHECK_EQ_U(_SW_ZERODIVIDE, 0x8); CHECK_EQ_U(_SW_INVALID, 0x10); CHECK_EQ_U(_SW_DENORMAL, 0x80000);
}
static volatile double v_one = 1.0, v_zero = 0.0, v_big = 1e308, v_tiny = 1e-200, v_three = 3.0;
TC_CASE(sw_observed_values)
{
    volatile double r;
    _clearfp(); r = v_one / v_three; (void)r; CHECK_EQ_U(_statusfp() & 0x1F, 0x1);
    _clearfp(); r = v_one / v_zero;  (void)r; CHECK_EQ_U(_statusfp() & 0x1F, 0x8);
    _clearfp(); r = v_big * v_big;   (void)r; CHECK_EQ_U(_statusfp() & 0x1F, 0x5);  /* overflow + inexact */
    _clearfp(); r = v_zero / v_zero; (void)r; CHECK_EQ_U(_statusfp() & 0x1F, 0x10);
    _clearfp(); r = v_tiny * v_tiny; (void)r; CHECK_EQ_U(_statusfp() & 0x1F, 0x3);  /* underflow + inexact */
    _clearfp();
}
TC_CASE(em_equal_sw)
{
    CHECK_EQ_U(_EM_INEXACT, _SW_INEXACT); CHECK_EQ_U(_EM_UNDERFLOW, _SW_UNDERFLOW);
    CHECK_EQ_U(_EM_OVERFLOW, _SW_OVERFLOW); CHECK_EQ_U(_EM_ZERODIVIDE, _SW_ZERODIVIDE);
    CHECK_EQ_U(_EM_INVALID, _SW_INVALID); CHECK_EQ_U(_EM_DENORMAL, _SW_DENORMAL);
}
TC_CASE(masks)
{
    CHECK_EQ_U(_MCW_EM, 0x0008001F); CHECK_EQ_U(_MCW_RC, 0x300); CHECK_EQ_U(_MCW_PC, 0x30000);
    CHECK_EQ_U(_MCW_DN, 0x03000000); CHECK_EQ_U(_MCW_IC, 0x40000);
}
TC_CASE(rc_values)
{
    CHECK_EQ_U(_RC_NEAR, 0); CHECK_EQ_U(_RC_DOWN, 0x100); CHECK_EQ_U(_RC_UP, 0x200); CHECK_EQ_U(_RC_CHOP, 0x300);
}
TC_CASE(controlfp_reflects_rc)
{
    unsigned orig = _controlfp(0, 0), r;
    r = _controlfp(_RC_UP, _MCW_RC);
    CHECK_EQ_U(r & _MCW_RC, _RC_UP);
    r = _controlfp(_RC_DOWN, _MCW_RC);
    CHECK_EQ_U(r & _MCW_RC, _RC_DOWN);
    r = _controlfp(_RC_CHOP, _MCW_RC);
    CHECK_EQ_U(r & _MCW_RC, _RC_CHOP);
    _controlfp(orig, _MCW_RC);
    CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, orig & _MCW_RC);
}
int main(void) { tc_begin(); TC_RUN(sw_values); TC_RUN(sw_observed_values); TC_RUN(em_equal_sw); TC_RUN(masks); TC_RUN(rc_values); TC_RUN(controlfp_reflects_rc); return tc_end(); }
