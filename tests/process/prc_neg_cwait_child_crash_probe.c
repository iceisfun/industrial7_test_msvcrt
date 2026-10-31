/* TC
 * id:    prc_neg_cwait_child_crash_probe
 * area:  process
 * funcs: _spawnl _cwait
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _spawn, _cwait
 * note:  PROBES: _cwait on a _P_NOWAIT handle of a child killed by an access violation returns that handle and sets
 *        termstat to 0xC0000005 (low 32 bits). Old CRTs may shift/encode termstat, hence crt >=140 only.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static char g_self[MAX_PATH];

TC_CASE(cwait_crash)
{
    volatile int m = _P_NOWAIT; intptr_t h, r; int st = 0;
    h = _spawnl(m, g_self, g_self, "--tc-child=crash", (char *)NULL);
    CHECK(h != -1);
    if (h == -1) return;
    r = _cwait(&st, h, 0);
    CHECK_EQ_I(r, h);
    CHECK_EQ_U((unsigned int)st, TC_STATUS_ACCESS_VIOLATION);
}

int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) { volatile int *volatile p = NULL; *p = 1; return 77; }
    GetModuleFileNameA(NULL, g_self, MAX_PATH);
    TC_RUN(cwait_crash);
    return tc_end();
}
