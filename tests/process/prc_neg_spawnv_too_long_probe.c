/* TC
 * id:    prc_neg_spawnv_too_long_probe
 * area:  process
 * kind:  negative
 * funcs: _spawnv
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _spawn functions (E2BIG: argument list exceeds 1024 bytes / environment too large)
 * note:  Probes how a 40000-character argument (passed after "/c" so cmd.exe exits if it ever starts) is rejected (CreateProcess limit is 32767 chars).
 *        Only -1 and nonzero errno are asserted strictly; the exact errno is probed.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>
#include <stdlib.h>

static char big[40001];

TC_CASE(too_long_arg)
{
    char cmd[MAX_PATH + 16]; const char *argv[4]; intptr_t r; int e, i, ok;
    GetSystemDirectoryA(cmd, MAX_PATH);
    for (i = 0; cmd[i]; i++) ;
    cmd[i++] = '\\'; cmd[i++] = 'c'; cmd[i++] = 'm'; cmd[i++] = 'd'; cmd[i++] = '.'; cmd[i++] = 'e'; cmd[i++] = 'x'; cmd[i++] = 'e'; cmd[i] = 0;
    for (i = 0; i < 40000; i++) big[i] = 'a';
    big[40000] = 0;
    argv[0] = cmd; argv[1] = "/c"; argv[2] = big; argv[3] = NULL;
    errno = 0;
    r = _spawnv(_P_WAIT, cmd, argv);
    e = errno;
    CHECK_EQ_I(r, -1);
    CHECK(e != 0);
    ok = (e == E2BIG || e == EINVAL || e == ENOENT);
    CHECK(ok);   /* probe */
}

int main(void) { tc_begin(); TC_RUN(too_long_arg); return tc_end(); }
