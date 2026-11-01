/* TC
 * id:    fpe_neg_statusfp_ignores_control_changes
 * area:  fpenv
 * kind:  negative
 * funcs: _statusfp _controlfp _clearfp
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _statusfp, _clearfp (flags are sticky until cleared); control word / status word are separate registers
 * note:  A: observed on Win10 19045 ucrtbase (raw: _statusfp() & _SW_ZERODIVIDE false right after _controlfp(_RC_UP,_MCW_RC)): _controlfp / _control87 / _controlfp_s clear the pending sticky status flags WHEN THEY CHANGE the control word; queries and same-value writes keep them. The old probe premise (never clears) was wrong. Was: PROBING that _controlfp never clears pending sticky flags (docs only say _clearfp clears them). Changing rounding / exception masks must neither set nor clear sticky status flags. Unmasking is NOT done while the flag is
 *        pending (that could trap on the next FP instruction); only RC and re-masking are exercised.
 */
#include "tc.h"
#include <float.h>
#ifndef _SW_INEXACT /* mingw float.h lacks the _SW_* aliases; values per MS float.h */
#define _SW_INEXACT 0x1
#define _SW_ZERODIVIDE 0x8
#endif
static volatile double v_zero = 0.0, v_one = 1.0, v_sink;

TC_CASE(flag_survives_non_changes)
{
    unsigned int old = _controlfp(0, 0);
    _controlfp(_MCW_EM, _MCW_EM);
    _controlfp(_RC_NEAR, _MCW_RC);
    _clearfp();
    v_sink = v_one / v_zero;
    CHECK(_statusfp() & _SW_ZERODIVIDE);
    (void)_controlfp(0, 0);                    /* query only */
    CHECK(_statusfp() & _SW_ZERODIVIDE);
    _controlfp(_MCW_EM, _MCW_EM);              /* same value */
    CHECK(_statusfp() & _SW_ZERODIVIDE);
    _controlfp(_RC_NEAR, _MCW_RC);             /* same value */
    CHECK(_statusfp() & _SW_ZERODIVIDE);
    _clearfp();
    CHECK_EQ_U(_statusfp(), 0);
    _controlfp(_RC_DOWN, _MCW_RC);
    _controlfp(_MCW_EM, _MCW_EM);
    CHECK_EQ_U(_statusfp(), 0);          /* control changes create no flags */
    _controlfp(old, _MCW_RC | _MCW_EM);
    _clearfp();
}
TC_CASE(flag_cleared_by_real_change)
{
    unsigned int old = _controlfp(0, 0);
    _controlfp(_MCW_EM, _MCW_EM);
    _controlfp(_RC_NEAR, _MCW_RC);
    _clearfp();
    v_sink = v_one / v_zero;
    CHECK(_statusfp() & _SW_ZERODIVIDE);
    _controlfp(_RC_UP, _MCW_RC);               /* observed: a call that CHANGES the control word clears the sticky flags */
    CHECK_EQ_U(_statusfp() & _SW_ZERODIVIDE, 0);
    v_sink = v_one / v_zero;
    _controlfp(_RC_UP, _MCW_RC);               /* no change: flag stays */
    CHECK(_statusfp() & _SW_ZERODIVIDE);
    _controlfp(_RC_NEAR, _MCW_RC);             /* change again */
    CHECK_EQ_U(_statusfp() & _SW_ZERODIVIDE, 0);
    _controlfp(old, _MCW_RC | _MCW_EM);
    _clearfp();
}
int main(void) { tc_begin(); TC_RUN(flag_survives_non_changes); TC_RUN(flag_cleared_by_real_change); return tc_end(); }
