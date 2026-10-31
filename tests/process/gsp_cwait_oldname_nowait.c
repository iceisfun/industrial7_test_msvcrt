/* TC
 * id:    gsp_cwait_oldname_nowait
 * area:  process
 * funcs: _cwait _spawnl
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _cwait: returns the handle of the terminated process and stores its result code in *termstat
 * note:  Unprefixed cwait/spawnl. Child (--tc-child=exit42) exits 42; 42 is used since low-byte semantics are not assumed.
 */
#include "tc.h"
#include <process.h>

static char g_self[MAX_PATH];
TC_CASE(nowait_cwait)
{
    int ts = -1; intptr_t h, r;
    h = spawnl(_P_NOWAIT, g_self, g_self, "--tc-child=exit42", (char *)NULL);
    CHECK(h > 0);
    if (h <= 0) return;
    r = cwait(&ts, h, 0);
    CHECK_EQ_I(r, h);
    CHECK_EQ_I(ts, 42);
}
TC_CASE(wait_child_action)
{
    int ts = -1; intptr_t h, r;
    h = spawnl(_P_NOWAIT, g_self, g_self, "--tc-child=exit42", (char *)NULL);
    CHECK(h > 0);
    if (h <= 0) return;
    r = cwait(&ts, h, _WAIT_CHILD);
    CHECK_EQ_I(r, h);
    CHECK_EQ_I(ts, 42);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return tc_strequ(c, "exit42") ? 42 : 90;
    GetModuleFileNameA(NULL, g_self, MAX_PATH);
    TC_RUN(nowait_cwait); TC_RUN(wait_child_action);
    return tc_end();
}
