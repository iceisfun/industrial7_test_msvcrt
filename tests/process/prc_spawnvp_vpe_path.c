/* TC
 * id:    prc_spawnvp_vpe_path
 * area:  process
 * funcs: _spawnvp _spawnvpe
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawnvp, _spawnvpe
 * note:  Assumes system32 on parent PATH. Explicit-envp + PATH search is probed in prc_spawnvpe_explicit_env_path_probe.
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
TC_CASE(vp)
{
    const char *av[] = { "cmd.exe", "/c", "exit 5", NULL };
    CHECK_EQ_I(_spawnvp(_P_WAIT, "cmd.exe", av), 5);
}
TC_CASE(vpe_inherit)
{
    const char *av[] = { "cmd.exe", "/c", "exit 5", NULL };
    CHECK_EQ_I(_spawnvpe(_P_WAIT, "cmd.exe", av, NULL), 5);
}
int main(void)
{
    tc_begin(); init_paths();
    TC_RUN(vp); TC_RUN(vpe_inherit);
    return tc_end();
}
