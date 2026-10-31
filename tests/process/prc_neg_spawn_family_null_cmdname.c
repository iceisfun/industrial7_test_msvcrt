/* TC
 * id:    prc_neg_spawn_family_null_cmdname
 * area:  process
 * funcs: _spawnle _spawnlp _spawnlpe _spawnv _spawnve _spawnvp _spawnvpe
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _spawn, _wspawn functions; Parameter validation (NULL cmdname -> invalid-parameter handler, EINVAL, -1)
 * note:  Existing test prc_neg_spawnl_null_cmdname covers only _spawnl. The control call uses _spawnv on cmd.exe /c exit 0.
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

static const char *volatile g_nul;
static const char *g_argv[3], *g_envp[2];

#define BAD_BEGIN() do { if (!tc_iph_install()) TC_SKIP("no iph"); tc_iph_count = 0; errno = 0; } while (0)
#define BAD_CHECK(r) do { CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(spawnle) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _spawnle(m, g_nul, "x", (char *)NULL, g_envp); BAD_CHECK(r); }
TC_CASE(spawnlp) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _spawnlp(m, g_nul, "x", (char *)NULL); BAD_CHECK(r); }
TC_CASE(spawnlpe) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _spawnlpe(m, g_nul, "x", (char *)NULL, g_envp); BAD_CHECK(r); }
TC_CASE(spawnv) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _spawnv(m, g_nul, g_argv); BAD_CHECK(r); }
TC_CASE(spawnve) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _spawnve(m, g_nul, g_argv, g_envp); BAD_CHECK(r); }
TC_CASE(spawnvp) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _spawnvp(m, g_nul, g_argv); BAD_CHECK(r); }
TC_CASE(spawnvpe) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _spawnvpe(m, g_nul, g_argv, g_envp); BAD_CHECK(r); }
TC_CASE(control_after)
{
    volatile int m = _P_WAIT; intptr_t r; const char *av[4];
    av[0] = g_cmd; av[1] = "/c"; av[2] = "exit 0"; av[3] = NULL; errno = 0;
    r = _spawnv(m, g_cmd, av);
    CHECK_EQ_I(r, 0);
}

int main(void)
{
    tc_begin(); init_paths();
    g_argv[0] = "x"; g_argv[1] = NULL; g_envp[0] = "TC_X=1"; g_envp[1] = NULL;
    TC_RUN(spawnle); TC_RUN(spawnlp); TC_RUN(spawnlpe); TC_RUN(spawnv); TC_RUN(spawnve);
    TC_RUN(spawnvp); TC_RUN(spawnvpe); TC_RUN(control_after);
    return tc_end();
}
