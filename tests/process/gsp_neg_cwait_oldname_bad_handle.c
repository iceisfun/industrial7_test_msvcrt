/* TC
 * id:    gsp_neg_cwait_oldname_bad_handle
 * area:  process
 * funcs: _cwait
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _cwait, _wait (returns -1 on error)
 * note:  Unprefixed cwait. The handle value -1 is deliberately NOT used: it is the current-process pseudo-handle, so waiting on
 *        it would block forever. errno value is not asserted here (see gsp_neg_cwait_oldname_errno_probe).
 *        Termstat-unmodified-on-failure is covered by prc_neg_cwait_failure_leaves_termstat; event handle in the probe test.
 */
#include "tc.h"
#include <process.h>

static volatile intptr_t g_h;

TC_CASE(bogus_handle) { int ts = 0x5A5A; g_h = (intptr_t)0x7ffffff4; CHECK_EQ_I(cwait(&ts, g_h, 0), -1); }
TC_CASE(null_handle)  { int ts = 0x5A5A; g_h = 0; CHECK_EQ_I(cwait(&ts, g_h, 0), -1); }
TC_CASE(closed_handle)
{
    int ts = 0x5A5A; HANDLE e = CreateEventA(NULL, TRUE, TRUE, NULL);
    CHECK(e != NULL);
    if (!e) return;
    CloseHandle(e);
    g_h = (intptr_t)e;
    CHECK_EQ_I(cwait(&ts, g_h, 0), -1);
}
int main(void) { tc_begin(); TC_RUN(bogus_handle); TC_RUN(null_handle); TC_RUN(closed_handle); return tc_end(); }
