/* TC
 * id:    gsp_neg_cwait_second_wait_probe
 * area:  process
 * funcs: _cwait _spawnl
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _cwait (does not state whether the handle is closed)
 * note:  PROBE: after a successful cwait the process handle is closed by the CRT, so waiting on the same handle again
 *        returns -1. Only the return value is asserted, not errno. The default invalid-parameter handler is not involved.
 */
#include "tc.h"
#include <process.h>

TC_CASE(second_cwait_fails)
{
    char self[MAX_PATH]; int ts = -1, ts2 = -1; intptr_t h, r1, r2;
    GetModuleFileNameA(NULL, self, MAX_PATH);
    h = spawnl(_P_NOWAIT, self, self, "--tc-child=exit42", (char *)NULL);
    CHECK(h > 0);
    if (h <= 0) return;
    r1 = cwait(&ts, h, 0);
    CHECK_EQ_I(r1, h);
    r2 = cwait(&ts2, h, 0);
    CHECK_EQ_I(r2, -1);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return tc_strequ(c, "exit42") ? 42 : 90;
    TC_RUN(second_cwait_fails);
    return tc_end();
}
