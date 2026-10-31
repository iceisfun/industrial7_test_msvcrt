/* TC
 * id:    prc_neg_exec_null_cmdname
 * area:  process
 * funcs: _execl _execv _execle _execve _execlp _execvp _wexecl _wexecv
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _exec, _wexec functions; Parameter validation (NULL cmdname -> handler, EINVAL, -1)
 * note:  Every call must return -1 (no overlay); if one did overlay, the process would exit without TC DONE and fail.
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

static const char *g_envp[2], *g_empty[1];
static const char *volatile g_nul;
static const wchar_t *volatile g_wnul;
static const wchar_t *g_wargv[2];

#define BAD_BEGIN() do { if (!tc_iph_install()) TC_SKIP("no iph"); tc_iph_count = 0; errno = 0; } while (0)
#define BAD_CHECK(r) do { CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(execl) { intptr_t r; BAD_BEGIN(); r = _execl(g_nul, "x", (char *)NULL); BAD_CHECK(r); }
TC_CASE(execv) { intptr_t r; BAD_BEGIN(); r = _execv(g_nul, g_empty); BAD_CHECK(r); }
TC_CASE(execle) { intptr_t r; BAD_BEGIN(); r = _execle(g_nul, "x", (char *)NULL, g_envp); BAD_CHECK(r); }
TC_CASE(execve) { intptr_t r; BAD_BEGIN(); r = _execve(g_nul, g_empty, g_envp); BAD_CHECK(r); }
TC_CASE(execlp) { intptr_t r; BAD_BEGIN(); r = _execlp(g_nul, "x", (char *)NULL); BAD_CHECK(r); }
TC_CASE(execvp) { intptr_t r; BAD_BEGIN(); r = _execvp(g_nul, g_empty); BAD_CHECK(r); }
TC_CASE(wexecl) { intptr_t r; BAD_BEGIN(); r = _wexecl(g_wnul, L"x", (wchar_t *)NULL); BAD_CHECK(r); }
TC_CASE(wexecv) { intptr_t r; BAD_BEGIN(); r = _wexecv(g_wnul, g_wargv); BAD_CHECK(r); }

int main(void)
{
    tc_begin(); init_paths();
    g_envp[0] = "TC_X=1"; g_envp[1] = NULL; g_empty[0] = NULL; g_wargv[0] = L"x"; g_wargv[1] = NULL;
    TC_RUN(execl); TC_RUN(execv); TC_RUN(execle); TC_RUN(execve); TC_RUN(execlp); TC_RUN(execvp);
    TC_RUN(wexecl); TC_RUN(wexecv);
    return tc_end();
}
