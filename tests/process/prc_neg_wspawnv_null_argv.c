/* TC
 * id:    prc_neg_wspawnv_null_argv
 * area:  process
 * funcs: _wspawnv _wspawnve _wspawnvp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _spawn, _wspawn functions; Parameter validation (argv NULL or argv[0] NULL is invalid)
 * note:  Uses a nonexistent cmdname so a CRT that skips the argv validation cannot launch an interactive cmd.exe.
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

static const wchar_t *g_empty[1], *g_envp[2];
static const wchar_t *const *volatile g_nullargv;

#define BAD_BEGIN() do { if (!tc_iph_install()) TC_SKIP("no iph"); tc_iph_count = 0; errno = 0; } while (0)
#define BAD_CHECK(r) do { CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(v_null_argv) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnv(m, L"C:\\tc_no_such_dir\\x.exe", g_nullargv); BAD_CHECK(r); }
TC_CASE(v_empty_argv) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnv(m, L"C:\\tc_no_such_dir\\x.exe", g_empty); BAD_CHECK(r); }
TC_CASE(vp_null_argv) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnvp(m, L"C:\\tc_no_such_dir\\x.exe", g_nullargv); BAD_CHECK(r); }
TC_CASE(vp_empty_argv) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnvp(m, L"C:\\tc_no_such_dir\\x.exe", g_empty); BAD_CHECK(r); }
TC_CASE(ve_null_argv) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnve(m, L"C:\\tc_no_such_dir\\x.exe", g_nullargv, g_envp); BAD_CHECK(r); }
TC_CASE(ve_empty_argv) { volatile int m = _P_WAIT; intptr_t r; BAD_BEGIN(); r = _wspawnve(m, L"C:\\tc_no_such_dir\\x.exe", g_empty, g_envp); BAD_CHECK(r); }

int main(void)
{
    tc_begin(); init_paths();
    g_empty[0] = NULL; g_envp[0] = L"TC_X=1"; g_envp[1] = NULL;
    TC_RUN(v_null_argv); TC_RUN(v_empty_argv); TC_RUN(vp_null_argv); TC_RUN(vp_empty_argv);
    TC_RUN(ve_null_argv); TC_RUN(ve_empty_argv);
    return tc_end();
}
