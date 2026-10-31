/* TC
 * id:    nfx_abort_behavior_mask_zero_probe
 * area:  negative_fatal
 * kind:  positive
 * funcs: 
 * procs: _set_abort_behavior
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_abort_behavior
 * note:  PROBE: with mask 0 the call changes nothing and returns the current flags, even when flags != 0
 *        (new = (old & ~mask) | (flags & mask) is inferred, not stated by the docs).
 */
#include "tc.h"
#include <stdlib.h>

typedef unsigned int (__cdecl *sab_t)(unsigned int, unsigned int);
TC_CASE(mask_zero)
{
    sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
    unsigned int orig;
    CHECK(sab);
    if (!sab) return;
    orig = sab(0, 3);
    CHECK_EQ_U(sab(3, 0), 0);
    CHECK_EQ_U(sab(0, 0), 0);
    sab(2, 2);
    CHECK_EQ_U(sab(3, 0), 2);
    CHECK_EQ_U(sab(0, 0), 2);
    sab(orig, 3);
}
int main(void)
{
    tc_begin();
    TC_RUN(mask_zero);
    return tc_end();
}
