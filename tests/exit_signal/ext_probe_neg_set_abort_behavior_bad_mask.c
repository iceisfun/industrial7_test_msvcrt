/* TC
 * id:    ext_probe_neg_set_abort_behavior_bad_mask
 * area:  exit_signal
 * funcs: _set_abort_behavior
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_abort_behavior (only two flags documented)
 * note:  Probes that bits outside _WRITE_ABORT_MSG/_CALL_REPORTFAULT are not validated: no invalid-parameter
 *        call and the bit is stored and reported back by the next call.
 */
#include "tc.h"
#include <signal.h>
#include <stdlib.h>

TC_CASE(bad_mask)
{
    unsigned orig = _set_abort_behavior(0, 0), prev;
    tc_iph_install(); tc_iph_count = 0;
    prev = _set_abort_behavior(0x8, 0x8);
    CHECK_EQ_U(prev, orig);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_U(_set_abort_behavior(0, 0) & 0x8, 0x8);
    _set_abort_behavior(orig, 0x8 | 0x3);
    CHECK_EQ_U(_set_abort_behavior(0, 0), orig);
}
int main(void) { tc_begin(); TC_RUN(bad_mask); return tc_end(); }
