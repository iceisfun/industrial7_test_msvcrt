/* TC
 * id:    prc_spawnvpe_explicit_env_path_probe
 * area:  process
 * funcs: _spawnvpe
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _spawnvpe (which PATH is searched is not stated)
 * note:  PROBES that _spawnvpe with an explicit envp lacking PATH/SystemRoot still finds cmd.exe via the parent's PATH
 *        and that cmd.exe runs with that minimal environment.
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
TC_CASE(vpe_explicit_env)
{
    const char *av[] = { "cmd.exe", "/c", "exit 6", NULL };
    const char *ev[] = { "TC_X=1", NULL };
    CHECK_EQ_I(_spawnvpe(_P_WAIT, "cmd.exe", av, ev), 6);
}
int main(void)
{
    tc_begin(); init_paths();
    TC_RUN(vpe_explicit_env);
    return tc_end();
}
