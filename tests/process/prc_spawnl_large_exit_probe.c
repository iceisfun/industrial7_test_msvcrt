/* TC
 * id:    prc_spawnl_large_exit_probe
 * area:  process
 * funcs: _spawnl
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _spawn
 * note:  Probes that the full 32-bit exit code of the child is returned by _P_WAIT (not masked to 8 bits).
 */
#include "tc.h"
#include <process.h>

static char cmd[MAX_PATH + 16];
static void one(const char *arg, int want)
{
    intptr_t r = _spawnl(_P_WAIT, cmd, cmd, "/c", arg, (char *)NULL);
    CHECK_EQ_I(r, want);
}
TC_CASE(large_codes) { one("exit 256", 256); one("exit 1000", 1000); one("exit 65535", 65535); }
int main(void)
{
    UINT n;
    tc_begin();
    n = GetSystemDirectoryA(cmd, MAX_PATH);
    { const char *s = "\\cmd.exe"; int i = 0; while (s[i]) { cmd[n + i] = s[i]; i++; } cmd[n + i] = 0; }
    TC_RUN(large_codes);
    return tc_end();
}
