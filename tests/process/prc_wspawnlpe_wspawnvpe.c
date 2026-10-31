/* TC
 * id:    prc_wspawnlpe_wspawnvpe
 * area:  process
 * funcs: _wspawnlpe _wspawnvpe
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _spawn, _wspawn functions
 * note:  PROBES that the p-variants with an explicit envp (lacking PATH and SystemRoot) still locate cmd.exe via the PARENT's PATH
 *        and that cmd.exe runs with that minimal environment. Docs do not say which PATH is searched.
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
TC_CASE(lpe)
{
    const wchar_t *ev[] = { L"TC_X=1", NULL };
    CHECK_EQ_I(_wspawnlpe(_P_WAIT, L"cmd.exe", L"cmd.exe", L"/c", L"exit 8", NULL, ev), 8);
}
TC_CASE(vpe)
{
    const wchar_t *av[] = { L"cmd.exe", L"/c", L"exit 9", NULL };
    const wchar_t *ev[] = { L"TC_X=1", NULL };
    CHECK_EQ_I(_wspawnvpe(_P_WAIT, L"cmd.exe", av, ev), 9);
}
int main(void)
{
    tc_begin(); init_paths();
    TC_RUN(lpe); TC_RUN(vpe);
    return tc_end();
}
