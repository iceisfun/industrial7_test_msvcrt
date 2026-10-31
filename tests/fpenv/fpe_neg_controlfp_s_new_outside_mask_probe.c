/* TC
 * id:    fpe_neg_controlfp_s_new_outside_mask_probe
 * area:  fpenv
 * funcs: _controlfp_s
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp_s
 * note:  Probe: new-value bits outside the mask. Either EINVAL, or 0 with only the RC field changed (EM must be unchanged).
 */
#include "tc.h"
#include <float.h>
#include <errno.h>

TC_CASE(outside_mask)
{
    unsigned orig = 0, c = 0, after = 0; int r;
    _controlfp_s(&orig, 0, 0);
    r = _controlfp_s(&c, 0xFFFFFFFF, _MCW_RC);
    CHECK(r == 0 || r == EINVAL);
    _controlfp_s(&after, 0, 0);
    if (r == 0) {
        CHECK_EQ_U(after & _MCW_RC, _RC_CHOP);
        CHECK_EQ_U(after & _MCW_EM, orig & _MCW_EM);
    }
    _controlfp_s(&c, orig, _MCW_RC | _MCW_EM);
}
int main(void) { tc_begin(); if (tc_iph_install()) {} TC_RUN(outside_mask); return tc_end(); }
