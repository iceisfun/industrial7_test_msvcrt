/* TC
 * id:    prc_cwait_null_termstat_probe
 * area:  process
 * funcs: _spawnl _cwait
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _cwait (termstat may be NULL: not stated explicitly)
 * note:  PROBES that _cwait(NULL, h, 0) is tolerated and returns the handle.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>
#include <stdlib.h>

static char g_cmd[MAX_PATH + 16], g_self[MAX_PATH];
static wchar_t g_wcmd[MAX_PATH + 16], g_wself[MAX_PATH];
static void init_paths(void)
{
    UINT n = GetSystemDirectoryA(g_cmd, MAX_PATH); lstrcpyA(g_cmd + n, "\\cmd.exe");
    n = GetSystemDirectoryW(g_wcmd, MAX_PATH); lstrcpyW(g_wcmd + n, L"\\cmd.exe");
    GetModuleFileNameA(NULL, g_self, MAX_PATH); GetModuleFileNameW(NULL, g_wself, MAX_PATH);
}
TC_CASE(null_termstat)
{
    intptr_t h = _spawnl(_P_NOWAIT, g_cmd, g_cmd, "/c", "exit 2", NULL);
    CHECK(h != -1);
    if (h == -1) return;
    CHECK_EQ_I(_cwait(NULL, h, 0), h);
}
int main(void)
{
    tc_begin(); init_paths();
    TC_RUN(null_termstat);
    return tc_end();
}
