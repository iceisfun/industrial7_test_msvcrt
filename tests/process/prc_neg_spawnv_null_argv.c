/* TC
 * id:    prc_neg_spawnv_null_argv
 * area:  process
 * funcs: _spawnv
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _spawn: NULL argv or argv[0] invokes the invalid-parameter handler; -1 / EINVAL
 * note:  Uses a valid cmdname (cmd.exe) so only argv can be the cause of the failure.
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

TC_CASE(argv_null)
{
    volatile int m = _P_WAIT; const char *const *volatile av = NULL; intptr_t r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _spawnv(m, cmdexe, av);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(argv0_null)
{
    volatile int m = _P_WAIT; const char *av[1]; intptr_t r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    av[0] = NULL; tc_iph_count = 0; errno = 0;
    r = _spawnv(m, cmdexe, av);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin(); find_cmd();
    TC_RUN(argv_null); TC_RUN(argv0_null);
    return tc_end();
}
