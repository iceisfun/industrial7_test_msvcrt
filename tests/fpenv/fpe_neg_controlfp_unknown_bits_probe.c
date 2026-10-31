/* TC
 * id:    fpe_neg_controlfp_unknown_bits_probe
 * area:  fpenv
 * funcs: _controlfp
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp (no error reporting, unlike _controlfp_s)
 * note:  Probe: _controlfp (non-_s) is believed to silently ignore undefined mask bits (no invalid-parameter call).
 */
#include "tc.h"
#include <float.h>

TC_CASE(undefined_mask)
{
    unsigned orig, after;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    orig = _controlfp(0, 0);
    tc_iph_count = 0;
    _controlfp(0xFFFFFFFF, 0x00004000);
    CHECK_EQ_I(tc_iph_count, 0);
    after = _controlfp(0, 0);
    CHECK_EQ_U(after & (_MCW_EM | _MCW_RC), orig & (_MCW_EM | _MCW_RC));
    _controlfp(orig, _MCW_EM | _MCW_RC);
}
int main(void) { tc_begin(); TC_RUN(undefined_mask); return tc_end(); }
