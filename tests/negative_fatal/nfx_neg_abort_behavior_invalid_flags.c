/* TC
 * id:    nfx_neg_abort_behavior_invalid_flags
 * area:  negative_fatal
 * funcs: _set_abort_behavior
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_abort_behavior
 * note:  PROBE: a mask with only an undefined bit (0x80000000) must not crash, returns the previous flags, and (since
 *        flags & mask == 0 and the old bits are kept) leaves the readable flags unchanged.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(undefined_mask)
{
    unsigned int base = _set_abort_behavior(0, 0);
    unsigned int prev = _set_abort_behavior(0x4, 0x80000000u);
    unsigned int after = _set_abort_behavior(0, 0);
    _set_abort_behavior(base, 0xFFFFFFFFu);
    CHECK_EQ_U(prev, base);
    CHECK_EQ_U(after, base);
    CHECK_EQ_U(_set_abort_behavior(0, 0), base);
}
int main(void) { tc_begin(); TC_RUN(undefined_mask); return tc_end(); }
