/* TC
 * id:    gpr_execl_family_neg_return_errno_enoent
 * area:  process
 * funcs: _execle _execlp _execlpe _execve _execvp _execvpe
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _exec, _wexec functions: returns -1, errno ENOENT for a file that is not found
 * note:  Complements prc_neg_execl_enoent (covers _execl,_execv,_wexecl,_execlp). Every call returns to the caller.
 */

#include "tc.h"
#include <process.h>
#include <errno.h>
#define MISSING "tc_no_such_program_zq91.exe"
#define MISSING_PATH "C:\\tc_no_such_dir\\x.exe"
static const char *const g_env[] = { "TC_X=1", NULL };
TC_CASE(execle_missing)
{ const char *volatile p = MISSING_PATH; intptr_t r; errno = 0; r = _execle(p, "x.exe", (char *)NULL, g_env); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(execlp_bare_missing)
{ const char *volatile p = MISSING; intptr_t r; errno = 0; r = _execlp(p, MISSING, (char *)NULL); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(execlpe_bare_missing)
{ const char *volatile p = MISSING; intptr_t r; errno = 0; r = _execlpe(p, MISSING, (char *)NULL, g_env); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(execve_missing)
{ const char *volatile p = MISSING_PATH; const char *av[] = { "x.exe", NULL }; intptr_t r; errno = 0; r = _execve(p, av, g_env); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(execvp_bare_missing)
{ const char *volatile p = MISSING; const char *av[] = { MISSING, NULL }; intptr_t r; errno = 0; r = _execvp(p, av); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(execvpe_bare_missing)
{ const char *volatile p = MISSING; const char *av[] = { MISSING, NULL }; intptr_t r; errno = 0; r = _execvpe(p, av, g_env); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(path_with_separator_missing_dir_not_searched_execvp)
{ const char *volatile p = MISSING_PATH; const char *av[] = { "x.exe", NULL }; intptr_t r; errno = 0; r = _execvp(p, av); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
int main(void)
{
    tc_begin();
    TC_RUN(execle_missing); TC_RUN(execlp_bare_missing); TC_RUN(execlpe_bare_missing); TC_RUN(execve_missing);
    TC_RUN(execvp_bare_missing); TC_RUN(execvpe_bare_missing); TC_RUN(path_with_separator_missing_dir_not_searched_execvp);
    return tc_end();
}
