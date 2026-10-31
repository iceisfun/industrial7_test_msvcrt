/* TC
 * id:    prc_spawnlpe_env
 * area:  process
 * funcs: _spawnlpe
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawnlpe
 * note:  Full path used with the p variant; child (this exe) checks the variable supplied via envp.
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
    DWORD r = GetEnvironmentVariableA("TC_PRC_E", b, sizeof b);
    return (r == 3 && b[0]=='y' && b[1]=='e' && b[2]=='s') ? 0 : 1;
}
TC_CASE(envp)
{
    char sr[MAX_PATH + 16] = "SystemRoot=";
    const char *ev[3];
    GetEnvironmentVariableA("SystemRoot", sr + 11, MAX_PATH);
    ev[0] = "TC_PRC_E=yes"; ev[1] = sr; ev[2] = NULL;
    CHECK_EQ_I(_spawnlpe(_P_WAIT, g_self, g_self, "--tc-child=e", NULL, ev), 0);
}
TC_CASE(parent_var_not_leaked)
{
    char sr[MAX_PATH + 16] = "SystemRoot=";
    const char *ev[3];
    GetEnvironmentVariableA("SystemRoot", sr + 11, MAX_PATH);
    ev[0] = "TC_PRC_OTHER=1"; ev[1] = sr; ev[2] = NULL;
    SetEnvironmentVariableA("TC_PRC_E", "yes");
    CHECK_EQ_I(_spawnlpe(_P_WAIT, g_self, g_self, "--tc-child=e", NULL, ev), 1);
}
int main(void)
{
    tc_begin(); init_paths();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(envp); TC_RUN(parent_var_not_leaked);
    return tc_end();
}
