/* TC
 * id:    prc_getpid_child_differs
 * area:  process
 * funcs: _getpid _spawnl _cwait
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _spawn functions, _cwait
 * note:  observed: the value returned by _spawnl(_P_NOWAIT) is a process handle usable with GetProcessId.
 *        Child is identified via --tc-child= and exits with its own _getpid().
 */
#include "tc.h"
#include <process.h>

static DWORD g_child_pid, g_child_code;

static void run_child(void)
{
    char exe[MAX_PATH]; intptr_t h; int st = 0;
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    h = _spawnl(_P_NOWAIT, exe, exe, "--tc-child=pid", (char *)NULL);
    if (h == -1) return;
    g_child_pid = GetProcessId((HANDLE)h);
    WaitForSingleObject((HANDLE)h, 10000);
    GetExitCodeProcess((HANDLE)h, &g_child_code);
    (void)st;
    _cwait(&st, h, 0);   /* closes the handle */
}

TC_CASE(spawn_and_compare)
{
    run_child();
    CHECK(g_child_pid != 0);
    CHECK(g_child_pid != GetCurrentProcessId());
    CHECK(g_child_pid != (DWORD)_getpid());
    CHECK_EQ_U(g_child_code, g_child_pid);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) _exit(_getpid());
    TC_RUN(spawn_and_compare);
    return tc_end();
}
