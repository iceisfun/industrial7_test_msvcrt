/* TC
 * id:    prc_wspawnlp_wspawnvp
 * area:  process
 * funcs: _wspawnlp _wspawnvp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawn, _wspawn functions: p variants search PATH
 * note:  Assumes system32 on PATH.
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
TC_CASE(lp) { CHECK_EQ_I(_wspawnlp(_P_WAIT, L"cmd.exe", L"cmd.exe", L"/c", L"exit 3", NULL), 3); }
TC_CASE(vp_noext)
{
    const wchar_t *av[] = { L"cmd", L"/c", L"exit 4", NULL };
    CHECK_EQ_I(_wspawnvp(_P_WAIT, L"cmd", av), 4);
}
int main(void)
{
    tc_begin(); init_paths();
    TC_RUN(lp); TC_RUN(vp_noext);
    return tc_end();
}
