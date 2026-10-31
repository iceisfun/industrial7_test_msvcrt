/* TC
 * id:    fpe_control87_rc_roundtrip
 * area:  fpenv
 * funcs: _control87
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _clearfp

 */
#include "tc.h"
#include <float.h>
#include <errno.h>

TC_CASE(four_modes)
{
    static const unsigned int modes[4] = { _RC_NEAR, _RC_DOWN, _RC_UP, _RC_CHOP };
    unsigned int orig = _control87(0, 0), r; int i;
    for (i = 0; i < 4; i++) {
        r = _control87(modes[i], _MCW_RC);
        CHECK_EQ_U(r & _MCW_RC, modes[i]);
        CHECK_EQ_U(_control87(0, 0) & _MCW_RC, modes[i]);
    }
    _control87(orig, _MCW_RC);
}
TC_CASE(restore)
{
    unsigned int orig = _control87(0, 0);
    _control87(_RC_UP, _MCW_RC);
    _control87(orig, _MCW_RC);
    CHECK_EQ_U(_control87(0, 0) & _MCW_RC, orig & _MCW_RC);
}
TC_CASE(other_bits_untouched)
{
    unsigned int orig = _control87(0, 0);
    _control87(_RC_DOWN, _MCW_RC);
    CHECK_EQ_U(_control87(0, 0) & 0x1f, orig & 0x1f);
    _control87(orig, _MCW_RC);
}
int main(void) { tc_begin(); TC_RUN(four_modes); TC_RUN(restore); TC_RUN(other_bits_untouched); return tc_end(); }
