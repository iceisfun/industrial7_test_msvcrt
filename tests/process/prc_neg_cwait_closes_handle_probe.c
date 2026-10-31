/* TC
 * id:    prc_neg_cwait_closes_handle_probe
 * area:  process
 * funcs: _spawnl _cwait
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _cwait
 * note:  PROBES that the CRT closes the process handle in a successful _cwait (so the handle is then invalid and a second _cwait fails with -1).
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
TC_CASE(handle_closed)
{
    int st = -1; intptr_t h; DWORD code;
    h = _spawnl(_P_NOWAIT, g_cmd, g_cmd, "/c", "exit 1", NULL);
    CHECK(h != -1);
    if (h == -1) return;
    CHECK_EQ_I(_cwait(&st, h, 0), h);
    SetLastError(0);
    CHECK_EQ_I(GetExitCodeProcess((HANDLE)h, &code), 0);
}
TC_CASE(second_cwait_fails)
{
    int st = -1; intptr_t h;
    h = _spawnl(_P_NOWAIT, g_cmd, g_cmd, "/c", "exit 1", NULL);
    CHECK(h != -1);
    if (h == -1) return;
    CHECK_EQ_I(_cwait(&st, h, 0), h);
    CHECK_EQ_I(_cwait(&st, h, 0), -1);
}
int main(void)
{
    tc_begin(); init_paths();
    TC_RUN(handle_closed); TC_RUN(second_cwait_fails);
    return tc_end();
}
