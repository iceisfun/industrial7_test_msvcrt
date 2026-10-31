/* TC
 * id:    gpr_execve_neg_null_args
 * area:  process
 * funcs: _execve _execvpe _wexecve _wexecvp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _exec, _wexec functions (parameter validation: NULL cmdname / argv / argv[0] -> EINVAL, -1, invalid parameter handler)
 * note:  envp == NULL is valid (inherit) and is probed in gpr_execve_null_envp_inherits_probe. Where cmdname is valid it names a nonexistent
 *        file so a CRT without validation fails with ENOENT instead of overlaying this process.
 */

#include "tc.h"
#include <process.h>
#include <errno.h>
static const char *g_empty[1];
static const char *const *volatile g_nullargv;
static const char *const g_env[] = { "TC_X=1", NULL };
static const wchar_t *g_wempty[1];
static const wchar_t *const *volatile g_wnullargv;
static const char *volatile g_nullname;
static const wchar_t *volatile g_wnullname;
#define BAD_BEGIN() do { if (!tc_iph_install()) TC_SKIP("no iph"); tc_iph_count = 0; errno = 0; } while (0)
#define BAD_CHECK(r) do { CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)
TC_CASE(execve_null_cmdname_einval_iph)
{ const char *av[] = { "x.exe", NULL }; intptr_t r; BAD_BEGIN(); r = _execve(g_nullname, av, g_env); BAD_CHECK(r); }
TC_CASE(execve_null_argv_einval_iph)
{ intptr_t r; BAD_BEGIN(); r = _execve("C:\\tc_no_such_dir\\x.exe", g_nullargv, g_env); BAD_CHECK(r); }
TC_CASE(execve_empty_argv_null_first_einval_iph)
{ intptr_t r; BAD_BEGIN(); r = _execve("C:\\tc_no_such_dir\\x.exe", g_empty, g_env); BAD_CHECK(r); }
TC_CASE(execvpe_null_cmdname)
{ const char *av[] = { "x.exe", NULL }; intptr_t r; BAD_BEGIN(); r = _execvpe(g_nullname, av, g_env); BAD_CHECK(r); }
TC_CASE(wexecve_null_cmdname)
{ const wchar_t *av[] = { L"x.exe", NULL }; const wchar_t *const e[] = { L"TC_X=1", NULL }; intptr_t r; BAD_BEGIN(); r = _wexecve(g_wnullname, av, e); BAD_CHECK(r); }
TC_CASE(wexecvp_null_argv)
{ intptr_t r; BAD_BEGIN(); r = _wexecvp(L"tc_no_such_program_zq91.exe", g_wnullargv); BAD_CHECK(r); }
int main(void)
{
    tc_begin(); g_empty[0] = NULL; g_wempty[0] = NULL; (void)g_wempty;
    TC_RUN(execve_null_cmdname_einval_iph); TC_RUN(execve_null_argv_einval_iph); TC_RUN(execve_empty_argv_null_first_einval_iph);
    TC_RUN(execvpe_null_cmdname); TC_RUN(wexecve_null_cmdname); TC_RUN(wexecvp_null_argv);
    return tc_end();
}
