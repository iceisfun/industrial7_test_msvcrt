/* TC
 * id:    prc_neg_spawn_wait_child_crash_probe
 * area:  process
 * funcs: _spawnl
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _spawn
 * note:  PROBES: a child killed by an access violation yields its NTSTATUS 0xC0000005 as the exit value: _spawnl(_P_WAIT) returns
 *        a value whose low 32 bits are 0xC0000005. (_cwait termstat is probed separately in prc_neg_cwait_child_crash_probe.)
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static char g_self[MAX_PATH];

TC_CASE(wait_crash)
{
    volatile int m = _P_WAIT; intptr_t r;
    r = _spawnl(m, g_self, g_self, "--tc-child=crash", (char *)NULL);
    CHECK_EQ_U((unsigned int)r, TC_STATUS_ACCESS_VIOLATION);
}
int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) { volatile int *volatile p = NULL; *p = 1; return 77; }
    GetModuleFileNameA(NULL, g_self, MAX_PATH);
    TC_RUN(wait_crash);
    return tc_end();
}
