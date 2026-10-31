/* TC
 * id:    prc_neg_spawnl_invalid_mode
 * area:  process
 * funcs: _spawnl
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _spawn: invalid mode => invalid-parameter handler, -1, EINVAL. Valid modes are _P_WAIT(0).._P_DETACH(4).
 * note:  PROBE: out-of-range modes (-1, 5, 100) give -1/EINVAL and call the invalid-parameter handler. Docs list EINVAL for a bad mode
 *        but do not specify that the handler runs before any process creation; kept out of the documented set.
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
static void try_mode(int mode)
{
    volatile int m = mode; intptr_t r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _spawnl(m, cmdexe, cmdexe, "/c", "exit", "0", NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(mode_minus1) { try_mode(-1); }
TC_CASE(mode_5)      { try_mode(5); }
TC_CASE(mode_100)    { try_mode(100); }

int main(void)
{
    tc_begin(); find_cmd();
    TC_RUN(mode_minus1); TC_RUN(mode_5); TC_RUN(mode_100);
    return tc_end();
}
