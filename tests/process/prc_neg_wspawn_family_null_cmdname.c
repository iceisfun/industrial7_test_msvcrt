/* TC
 * id:    prc_neg_wspawn_family_null_cmdname
 * area:  process
 * funcs: _wspawnle _wspawnlp _wspawnlpe _wspawnv _wspawnve _wspawnvp _wspawnvpe
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _spawn, _wspawn functions; Parameter validation (NULL cmdname -> invalid-parameter handler, EINVAL, -1)
 * note:  Control call uses _wspawnv on cmd.exe /c exit 0.
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

static const wchar_t *volatile g_nul;
static const wchar_t *g_argv[3], *g_envp[2];

#define BAD_BEGIN() do { if (!tc_iph_install()) TC_SKIP("no iph"); tc_iph_count = 0; errno = 0; } while (0)
#define BAD_CHECK(r) do { CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(wspawnle) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnle(m, g_nul, L"x", (wchar_t *)NULL, g_envp); BAD_CHECK(r); }
TC_CASE(wspawnlp) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnlp(m, g_nul, L"x", (wchar_t *)NULL); BAD_CHECK(r); }
TC_CASE(wspawnlpe) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnlpe(m, g_nul, L"x", (wchar_t *)NULL, g_envp); BAD_CHECK(r); }
TC_CASE(wspawnv) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnv(m, g_nul, g_argv); BAD_CHECK(r); }
TC_CASE(wspawnve) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnve(m, g_nul, g_argv, g_envp); BAD_CHECK(r); }
TC_CASE(wspawnvp) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnvp(m, g_nul, g_argv); BAD_CHECK(r); }
TC_CASE(wspawnvpe) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnvpe(m, g_nul, g_argv, g_envp); BAD_CHECK(r); }
TC_CASE(control_after)
{
    volatile int m = _P_WAIT; intptr_t r; const wchar_t *av[4];
    av[0] = g_wcmd; av[1] = L"/c"; av[2] = L"exit 0"; av[3] = NULL; errno = 0;
    r = _wspawnv(m, g_wcmd, av);
    CHECK_EQ_I(r, 0);
}

int main(void)
{
    tc_begin(); init_paths();
    g_argv[0] = L"x"; g_argv[1] = NULL; g_envp[0] = L"TC_X=1"; g_envp[1] = NULL;
    TC_RUN(wspawnle); TC_RUN(wspawnlp); TC_RUN(wspawnlpe); TC_RUN(wspawnv); TC_RUN(wspawnve);
    TC_RUN(wspawnvp); TC_RUN(wspawnvpe); TC_RUN(control_after);
    return tc_end();
}
