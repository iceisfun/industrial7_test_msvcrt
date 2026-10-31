/* TC
 * id:    prc_spawn_nowait_cwait
 * area:  process
 * funcs: _spawnl _cwait
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _cwait: returns the handle of the terminated process on success
 * note:  Only the return value (the handle) is asserted; the value stored in termstat is probed in prc_cwait_termstat_value_probe
 *        because the docs do not unambiguously say whether it is the raw exit code.
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
static void one(int code, const char *arg)
{
    int st = -1; intptr_t h, r;
    h = _spawnl(_P_NOWAIT, g_cmd, g_cmd, "/c", arg, NULL);
    CHECK(h != -1);
    if (h == -1) return;
    r = _cwait(&st, h, 0);
    CHECK_EQ_I(r, h);
    (void)code; /* termstat encoding is asserted separately in prc_cwait_termstat_value_probe */
}
TC_CASE(exit5)   { one(5, "exit 5"); }
TC_CASE(exit0)   { one(0, "exit 0"); }
TC_CASE(exit255) { one(255, "exit 255"); }
int main(void)
{
    tc_begin(); init_paths();
    TC_RUN(exit5); TC_RUN(exit0); TC_RUN(exit255);
    return tc_end();
}
