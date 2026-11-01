/* TC
 * id:    prc_neg_cwait_failure_leaves_termstat
 * area:  process
 * funcs: _cwait
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _cwait (returns -1 on error)
 * note:  OBSERVED on Win10 22H2 (19045): when _cwait fails (invalid handle 0x7ffffff4, or handle 0 with _WAIT_GRANDCHILD) it
 *        returns -1 AND stores -1 into *termstat; it does not leave the sentinel untouched (the test id is historical). Stable.
 */
#include "tc.h"
#include <process.h>

TC_CASE(invalid_handle_sentinel)
{
    int st = 0x5A5A; intptr_t r = _cwait(&st, (intptr_t)0x7ffffff4, 0);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(st, -1);   /* observed: failure stores -1 */
}
TC_CASE(zero_handle_grandchild_sentinel)
{
    int st = 0x5A5A; intptr_t r = _cwait(&st, 0, _WAIT_GRANDCHILD);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(st, -1);   /* observed: failure stores -1 */
}
int main(void) { tc_begin(); TC_RUN(invalid_handle_sentinel); TC_RUN(zero_handle_grandchild_sentinel); return tc_end(); }
