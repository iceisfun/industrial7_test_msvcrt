/* TC
 * id:    prc_spawn_inherit_env
 * area:  process
 * funcs: _spawnl
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawnl: child inherits the parent environment
 * note:  _putenv is used only as a trivial helper to set the variable. Child is this exe.
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
static int child_main(const char *n)
{
    char b[16]; (void)n;
    DWORD r = GetEnvironmentVariableA("TC_PRC_INH", b, sizeof b);
    return (r == 1 && b[0] == '1') ? 0 : 1;
}
TC_CASE(inherit)
{
    intptr_t r;
    CHECK_EQ_I(_putenv("TC_PRC_INH=1"), 0);
    r = _spawnl(_P_WAIT, g_self, g_self, "--tc-child=inh", NULL);
    CHECK_EQ_I(r, 0);
}
TC_CASE(not_set_not_seen)
{
    intptr_t r;
    CHECK_EQ_I(_putenv("TC_PRC_INH="), 0);
    r = _spawnl(_P_WAIT, g_self, g_self, "--tc-child=inh", NULL);
    CHECK_EQ_I(r, 1);
}
int main(void)
{
    tc_begin(); init_paths();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(inherit); TC_RUN(not_set_not_seen);
    return tc_end();
}
