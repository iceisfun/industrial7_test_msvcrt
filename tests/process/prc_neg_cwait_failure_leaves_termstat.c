/* TC
 * id:    prc_neg_cwait_failure_leaves_termstat
 * area:  process
 * funcs: _cwait
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _cwait (returns -1 on error)
 * note:  PROBE: the docs do not say that *termstat is left unmodified when _cwait fails. Return value -1 is documented and is
 *        asserted in prc_neg_cwait_bad_handle; this test probes only the sentinel being preserved.
 */
#include "tc.h"
#include <process.h>

TC_CASE(invalid_handle_sentinel)
{
    int st = 0x5A5A; intptr_t r = _cwait(&st, (intptr_t)0x7ffffff4, 0);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(st, 0x5A5A);
}
TC_CASE(zero_handle_grandchild_sentinel)
{
    int st = 0x5A5A; intptr_t r = _cwait(&st, 0, _WAIT_GRANDCHILD);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(st, 0x5A5A);
}
int main(void) { tc_begin(); TC_RUN(invalid_handle_sentinel); TC_RUN(zero_handle_grandchild_sentinel); return tc_end(); }
