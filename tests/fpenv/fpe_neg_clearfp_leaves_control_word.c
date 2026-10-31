/* TC
 * id:    fpe_neg_clearfp_leaves_control_word
 * area:  fpenv
 * kind:  negative
 * funcs: _clearfp _controlfp _statusfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _clearfp (clears the floating-point status word), _statusfp
 * note:  _clearfp must not touch the control word (RC/EM). Exact flag set returned is only checked for inexact being included.
 */
#include "tc.h"
#include <float.h>
#ifndef _SW_INEXACT /* mingw float.h lacks the _SW_* aliases; values per MS float.h */
#define _SW_INEXACT 0x1
#define _SW_ZERODIVIDE 0x8
#endif
static volatile double v_one = 1.0, v_three = 3.0, v_sink;

TC_CASE(control_unchanged)
{
    unsigned int old = _controlfp(0, 0), before, after, st;
    _controlfp(_RC_CHOP, _MCW_RC);
    _controlfp(_MCW_EM & ~_EM_ZERODIVIDE, _MCW_EM);
    _clearfp();
    v_sink = v_one / v_three;               /* inexact, masked */
    before = _controlfp(0, 0);
    st = _clearfp();
    after = _controlfp(0, 0);
    CHECK(st & _SW_INEXACT);
    CHECK_EQ_U(after, before);
    CHECK_EQ_U(after & _MCW_RC, _RC_CHOP);
    CHECK_EQ_U(after & _MCW_EM, _MCW_EM & ~_EM_ZERODIVIDE);
    CHECK_EQ_U(_statusfp(), 0);
    _controlfp(old, _MCW_RC | _MCW_EM);
}
int main(void) { tc_begin(); TC_RUN(control_unchanged); return tc_end(); }
