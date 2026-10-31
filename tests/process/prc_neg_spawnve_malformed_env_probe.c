/* TC
 * id:    prc_neg_spawnve_malformed_env_probe
 * area:  process
 * funcs: _spawnve _wspawnve
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _spawnve, _wspawnve (envp is a NULL-terminated array of "NAME=value" strings)
 * note:  PROBE: (1) an envp entry with no '=' is either passed through (child runs, return 0) or rejected (-1/EINVAL).
 *        (2) an empty envp array {NULL} either launches cmd.exe /c exit 0 (return 0) or fails (-1). Only "no crash" and
 *        these outcome sets are asserted; the exact behaviour is what the Windows run is meant to reveal.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static char g_cmd[MAX_PATH + 16];
static wchar_t g_wcmd[MAX_PATH + 16];
static void init_paths(void)
{
    UINT n = GetSystemDirectoryA(g_cmd, MAX_PATH); lstrcpyA(g_cmd + n, "\\cmd.exe");
    n = GetSystemDirectoryW(g_wcmd, MAX_PATH); lstrcpyW(g_wcmd + n, L"\\cmd.exe");
}
TC_CASE(noequals)
{
    const char *av[] = { g_cmd, "/c", "exit 0", NULL };
    const char *ev[] = { "NOEQUALS", NULL };
    intptr_t r; errno = 0;
    r = _spawnve(_P_WAIT, g_cmd, av, ev);
    CHECK(r == 0 || (r == -1 && errno == EINVAL));
}
TC_CASE(noequals_w)
{
    const wchar_t *av[] = { g_wcmd, L"/c", L"exit 0", NULL };
    const wchar_t *ev[] = { L"NOEQUALS", NULL };
    intptr_t r; errno = 0;
    r = _wspawnve(_P_WAIT, g_wcmd, av, ev);
    CHECK(r == 0 || (r == -1 && errno == EINVAL));
}
TC_CASE(empty_env)
{
    const char *av[] = { g_cmd, "/c", "exit 0", NULL };
    const char *ev[] = { NULL };
    intptr_t r = _spawnve(_P_WAIT, g_cmd, av, ev);
    CHECK(r == 0 || r == -1);
}
TC_CASE(empty_env_w)
{
    const wchar_t *av[] = { g_wcmd, L"/c", L"exit 0", NULL };
    const wchar_t *ev[] = { NULL };
    intptr_t r = _wspawnve(_P_WAIT, g_wcmd, av, ev);
    CHECK(r == 0 || r == -1);
}
int main(void)
{
    tc_begin(); init_paths();
    TC_RUN(noequals); TC_RUN(noequals_w); TC_RUN(empty_env); TC_RUN(empty_env_w);
    return tc_end();
}
