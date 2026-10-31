/* TC
 * id:    prc_wspawnv_wspawnve
 * area:  process
 * funcs: _wspawnv _wspawnve
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
    if (tc_strequ(n, "wenv")) {
        wchar_t b[16];
        DWORD r = GetEnvironmentVariableW(L"TC_PRC_W", b, 16);
        return (r == 4 && tc_wcsequ(b, L"wide")) ? 0 : 1;
    } else {
        const wchar_t *c = GetCommandLineW();
        static const wchar_t k[] = L"caf\x00e9";
        for (; *c; c++) { int i = 0; while (k[i] && c[i] == k[i]) i++; if (!k[i]) return 0; }
        return 1;
    }
}
TC_CASE(v_exit11)
{
    const wchar_t *av[] = { g_wcmd, L"/c", L"exit 11", NULL };
    CHECK_EQ_I(_wspawnv(_P_WAIT, g_wcmd, av), 11);
}
TC_CASE(ve_env)
{
    wchar_t sr[MAX_PATH + 16] = L"SystemRoot=";
    const wchar_t *av[] = { g_wself, L"--tc-child=wenv", NULL };
    const wchar_t *ev[3];
    GetEnvironmentVariableW(L"SystemRoot", sr + 11, MAX_PATH);
    ev[0] = L"TC_PRC_W=wide"; ev[1] = sr; ev[2] = NULL;
    CHECK_EQ_I(_wspawnve(_P_WAIT, g_wself, av, ev), 0);
}
TC_CASE(nonascii_arg)
{
    const wchar_t *av[] = { g_wself, L"--tc-child=warg", L"caf\x00e9", NULL };
    CHECK_EQ_I(_wspawnv(_P_WAIT, g_wself, av), 0);
}
int main(void)
{
    tc_begin(); init_paths();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(v_exit11); TC_RUN(ve_env); TC_RUN(nonascii_arg);
    return tc_end();
}
