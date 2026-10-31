/* TC
 * id:    prc_wspawnl_wait
 * area:  process
 * funcs: _wspawnl
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawn, _wspawn functions
 * note:  -
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
static intptr_t run(const wchar_t *a) { return _wspawnl(_P_WAIT, g_wcmd, g_wcmd, L"/c", a, NULL); }
TC_CASE(exit0)   { CHECK_EQ_I(run(L"exit 0"), 0); }
TC_CASE(exit9)   { CHECK_EQ_I(run(L"exit 9"), 9); }
TC_CASE(exit255) { CHECK_EQ_I(run(L"exit 255"), 255); }
int main(void)
{
    tc_begin(); init_paths();
    TC_RUN(exit0); TC_RUN(exit9); TC_RUN(exit255);
    return tc_end();
}
