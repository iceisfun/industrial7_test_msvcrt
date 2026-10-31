/* TC
 * id:    prc_spawnv_wait
 * area:  process
 * funcs: _spawnv
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawnv
 * note:  
 */
#include "tc.h"
#include <process.h>

static char cmd[MAX_PATH + 16];
TC_CASE(argv_exit3)
{
    const char *av[] = { cmd, "/c", "exit 3", NULL };
    intptr_t r = _spawnv(_P_WAIT, cmd, av);
    CHECK_EQ_I(r, 3);
}
TC_CASE(argv_exit0)
{
    const char *av[] = { cmd, "/c", "exit 0", NULL };
    intptr_t r = _spawnv(_P_WAIT, cmd, av);
    CHECK_EQ_I(r, 0);
}
int main(void)
{
    UINT n;
    tc_begin();
    n = GetSystemDirectoryA(cmd, MAX_PATH);
    { const char *s = "\\cmd.exe"; int i = 0; while (s[i]) { cmd[n + i] = s[i]; i++; } cmd[n + i] = 0; }
    TC_RUN(argv_exit3); TC_RUN(argv_exit0);
    return tc_end();
}
