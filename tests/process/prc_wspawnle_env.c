/* TC
 * id:    prc_wspawnle_env
 * area:  process
 * funcs: _wspawnle
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawn, _wspawn functions
 * note:  Child is this exe.
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
    wchar_t b[16]; (void)n;
    DWORD r = GetEnvironmentVariableW(L"TC_PRC_WLE", b, 16);
    return (r == 2 && tc_wcsequ(b, L"ok")) ? 0 : 1;
}
TC_CASE(envp)
{
    wchar_t sr[MAX_PATH + 16] = L"SystemRoot=";
    const wchar_t *ev[3];
    GetEnvironmentVariableW(L"SystemRoot", sr + 11, MAX_PATH);
    ev[0] = L"TC_PRC_WLE=ok"; ev[1] = sr; ev[2] = NULL;
    CHECK_EQ_I(_wspawnle(_P_WAIT, g_wself, g_wself, L"--tc-child=x", NULL, ev), 0);
}
int main(void)
{
    tc_begin(); init_paths();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(envp);
    return tc_end();
}
