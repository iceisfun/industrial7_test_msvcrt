/* TC
 * id:    fpe_neg_statusfp_ignores_control_changes
 * area:  fpenv
 * kind:  negative
 * funcs: _statusfp _controlfp _clearfp
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _statusfp, _clearfp (flags are sticky until cleared); control word / status word are separate registers
 * note:  PROBING that _controlfp never clears pending sticky flags (docs only say _clearfp clears them). Changing rounding / exception masks must neither set nor clear sticky status flags. Unmasking is NOT done while the flag is
 *        pending (that could trap on the next FP instruction); only RC and re-masking are exercised.
 */
#include "tc.h"
#include <float.h>
#ifndef _SW_INEXACT /* mingw float.h lacks the _SW_* aliases; values per MS float.h */
#define _SW_INEXACT 0x1
#define _SW_ZERODIVIDE 0x8
#endif
static volatile double v_zero = 0.0, v_one = 1.0, v_sink;

TC_CASE(flag_survives_control_changes)
{
    unsigned int old = _controlfp(0, 0);
    _controlfp(_MCW_EM, _MCW_EM);
    _clearfp();
    v_sink = v_one / v_zero;
    CHECK(_statusfp() & _SW_ZERODIVIDE);
    _controlfp(_RC_UP, _MCW_RC);
    CHECK(_statusfp() & _SW_ZERODIVIDE);
    _controlfp(_MCW_EM, _MCW_EM);
    CHECK(_statusfp() & _SW_ZERODIVIDE);
    _controlfp(_RC_NEAR, _MCW_RC);
    CHECK(_statusfp() & _SW_ZERODIVIDE);
    _clearfp();
    CHECK_EQ_U(_statusfp(), 0);
    _controlfp(_RC_DOWN, _MCW_RC);
    _controlfp(_MCW_EM, _MCW_EM);
    CHECK_EQ_U(_statusfp(), 0);          /* control changes create no flags */
    _controlfp(old, _MCW_RC | _MCW_EM);
    _clearfp();
}
int main(void) { tc_begin(); TC_RUN(flag_survives_control_changes); return tc_end(); }
