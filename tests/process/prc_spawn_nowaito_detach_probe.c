/* TC
 * id:    prc_spawn_nowaito_detach_probe
 * area:  process
 * funcs: _spawnl
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _spawn, _wspawn functions (_P_NOWAIT, _P_NOWAITO, _P_DETACH)
 * note:  Probes the return values of asynchronous spawn modes, whose docs are vague: only "!= -1" is asserted.
 *        _P_DETACH may behave differently depending on whether the parent has a console.
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

TC_CASE(nowaito)
{
    volatile int mode = _P_NOWAITO; intptr_t r;
    r = _spawnl(mode, cmdexe, cmdexe, "/c", "exit", "0", NULL);
    CHECK(r != -1);
    if (r != -1 && r != 0) { WaitForSingleObject((HANDLE)r, 10000); CloseHandle((HANDLE)r); }
}

TC_CASE(detach)
{
    volatile int mode = _P_DETACH; intptr_t r;
    r = _spawnl(mode, cmdexe, cmdexe, "/c", "exit", "0", NULL);
    CHECK(r != -1);
}

int main(void)
{
    tc_begin(); find_cmd();
    TC_RUN(nowaito);
    TC_RUN(detach);
    return tc_end();
}
