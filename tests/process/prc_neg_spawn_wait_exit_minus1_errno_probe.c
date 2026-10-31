/* TC
 * id:    prc_neg_spawn_wait_exit_minus1_errno_probe
 * area:  process
 * funcs: _spawnl system
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _spawn, system (-1 is the error return)
 * note:  PROBES: after a child that exited with code -1 (not a spawn failure), errno is still 0 (neither _spawnl nor system sets it).
 */
#include "tc.h"
#include <process.h>
#include <errno.h>
#include <stdlib.h>

static char g_cmd[MAX_PATH + 16];

TC_CASE(spawnl_exit_m1)
{
    volatile int m = _P_WAIT; intptr_t r; errno = 0;
    r = _spawnl(m, g_cmd, g_cmd, "/c", "exit -1", (char *)NULL);
    CHECK_EQ_I(errno, 0);
}
TC_CASE(system_exit_m1)
{
    const char *volatile c = "exit -1"; int r; errno = 0;
    r = system(c);
    CHECK_EQ_I(errno, 0);
}

int main(void)
{
    UINT n;
    tc_begin();
    n = GetSystemDirectoryA(g_cmd, MAX_PATH); lstrcpyA(g_cmd + n, "\\cmd.exe");
    TC_RUN(spawnl_exit_m1); TC_RUN(system_exit_m1);
    return tc_end();
}
