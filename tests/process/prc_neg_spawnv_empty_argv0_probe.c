/* TC
 * id:    prc_neg_spawnv_empty_argv0_probe
 * area:  process
 * funcs: _spawnv
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _spawn (parameter validation)
 * note:  Probes whether an empty-string argv[0] is rejected as an invalid parameter (-1, EINVAL, handler called).
 *        Uses _P_NOWAIT and terminates the child if it was (unexpectedly) created, so cmd.exe cannot block on stdin.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static char cmdexe[MAX_PATH + 32];
static void find_cmd(void)
{
    const char *tail = "\\system32\\cmd.exe"; DWORD n = GetEnvironmentVariableA("SystemRoot", cmdexe, MAX_PATH); int i;
    if (n == 0 || n >= MAX_PATH) { cmdexe[0] = 'C'; cmdexe[1] = ':'; cmdexe[2] = '\\'; cmdexe[3] = 'W'; cmdexe[4] = 0; n = 4; }
    for (i = 0; tail[i]; i++) cmdexe[n + i] = tail[i];
    cmdexe[n + i] = 0;
}

TC_CASE(empty_argv0)
{
    volatile int m = _P_NOWAIT; const char *av[2]; intptr_t r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    av[0] = ""; av[1] = NULL; tc_iph_count = 0; errno = 0;
    r = _spawnv(m, cmdexe, av);
    if (r != -1 && r != 0) { TerminateProcess((HANDLE)r, 1); CloseHandle((HANDLE)r); }
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin(); find_cmd();
    TC_RUN(empty_argv0);
    return tc_end();
}
