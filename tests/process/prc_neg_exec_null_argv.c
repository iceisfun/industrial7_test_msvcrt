/* TC
 * id:    prc_neg_exec_null_argv
 * area:  process
 * funcs: _execv
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _exec, _wexec functions; Parameter validation (argv / argv[0] -> handler, EINVAL, -1)
 * note:  Every call must return -1 (no overlay). A nonexistent cmdname is used so that a CRT that skips argv validation fails with ENOENT
 *        rather than overlaying this process with an interactive cmd.exe.
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
static const char *const *volatile g_nullargv;




#define BAD_BEGIN() do { if (!tc_iph_install()) TC_SKIP("no iph"); tc_iph_count = 0; errno = 0; } while (0)
#define BAD_CHECK(r) do { CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(execv_null_argv) { intptr_t r; BAD_BEGIN(); r = _execv("C:\\tc_no_such_dir\\x.exe", g_nullargv); BAD_CHECK(r); }
TC_CASE(execv_empty_argv) { intptr_t r; BAD_BEGIN(); r = _execv("C:\\tc_no_such_dir\\x.exe", g_empty); BAD_CHECK(r); }

int main(void)
{
    tc_begin(); init_paths();
    g_envp[0] = "TC_X=1"; g_envp[1] = NULL; g_empty[0] = NULL; 
    TC_RUN(execv_null_argv); TC_RUN(execv_empty_argv);
    return tc_end();
}
