/* TC
 * id:    gpr_wexec_neg_enoent
 * area:  process
 * funcs: _wexecle _wexeclp _wexecve _wexecvp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _exec, _wexec functions: returns -1, errno ENOENT; the caller survives
 */

#include "tc.h"
#include <process.h>
#include <errno.h>
#define MISSING L"tc_no_such_program_zq91.exe"
#define MISSING_PATH L"C:\\tc_no_such_dir\\x.exe"
static const wchar_t *const g_env[] = { L"TC_X=1", NULL };
TC_CASE(wexecle_missing)
{ const wchar_t *volatile p = MISSING_PATH; intptr_t r; errno = 0; r = _wexecle(p, L"x.exe", (wchar_t *)NULL, g_env); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(wexeclp_missing)
{ const wchar_t *volatile p = MISSING; intptr_t r; errno = 0; r = _wexeclp(p, MISSING, (wchar_t *)NULL); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(wexecve_missing)
{ const wchar_t *volatile p = MISSING_PATH; const wchar_t *av[] = { L"x.exe", NULL }; intptr_t r; errno = 0; r = _wexecve(p, av, g_env); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(wexecvp_missing)
{ const wchar_t *volatile p = MISSING; const wchar_t *av[] = { MISSING, NULL }; intptr_t r; errno = 0; r = _wexecvp(p, av); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(wexecvp_missing_does_not_modify_argv_array)
{
    const wchar_t *volatile p = MISSING; const wchar_t *a0 = MISSING, *a1 = L"arg1"; const wchar_t *av[3]; intptr_t r;
    av[0] = a0; av[1] = a1; av[2] = NULL; errno = 0;
    r = _wexecvp(p, av);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_P(av[0], a0); CHECK_EQ_P(av[1], a1); CHECK_EQ_P(av[2], NULL);
    CHECK_WSTR(av[1], L"arg1");
}
int main(void)
{
    tc_begin();
    TC_RUN(wexecle_missing); TC_RUN(wexeclp_missing); TC_RUN(wexecve_missing); TC_RUN(wexecvp_missing);
    TC_RUN(wexecvp_missing_does_not_modify_argv_array);
    return tc_end();
}
