/* TC
 * id:    prc_spawnl_wait_exit_codes
 * area:  process
 * funcs: _spawnl
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawn: _P_WAIT returns the exit status of the child
 * note:  cmd path from GetSystemDirectoryA.
 */
#include "tc.h"
#include <process.h>

static char cmd[MAX_PATH + 16];
static void one(const char *arg, int want)
{
    intptr_t r = _spawnl(_P_WAIT, cmd, cmd, "/c", arg, (char *)NULL);
    CHECK_EQ_I(r, want);
}
TC_CASE(exit_codes)
{
    one("exit 0", 0); one("exit 1", 1); one("exit 7", 7); one("exit 200", 200); one("exit 255", 255);
}
int main(void)
{
    UINT n;
    tc_begin();
    n = GetSystemDirectoryA(cmd, MAX_PATH);
    cmd[n] = 0;
    { const char *s = "\\cmd.exe"; int i = 0; while (s[i]) { cmd[n + i] = s[i]; i++; } cmd[n + i] = 0; }
    TC_RUN(exit_codes);
    return tc_end();
}
