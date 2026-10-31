/* TC
 * id:    prc_neg_execl_enoent
 * area:  process
 * funcs: _execl _execv _wexecl _execlp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _exec, _wexec functions: returns -1 on error; errno ENOENT = file or path not found
 * note:  Each call returns to the caller (no overlay) and the test keeps running.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(execl_missing)
{
    const char *volatile p = "C:\\tc_no_such_dir\\x.exe"; intptr_t r; errno = 0;
    r = _execl(p, "x.exe", (char *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(execv_missing)
{
    const char *volatile p = "C:\\tc_no_such_dir\\x.exe"; const char *av[2]; intptr_t r;
    av[0] = "x.exe"; av[1] = NULL; errno = 0;
    r = _execv(p, av);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(wexecl_missing)
{
    const wchar_t *volatile p = L"C:\\tc_no_such_dir\\x.exe"; intptr_t r; errno = 0;
    r = _wexecl(p, L"x.exe", (wchar_t *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(execlp_bare_missing)
{
    const char *volatile p = "tc_no_such_program_zq91.exe"; intptr_t r; errno = 0;
    r = _execlp(p, "tc_no_such_program_zq91.exe", (char *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}

int main(void)
{
    tc_begin();
    TC_RUN(execl_missing); TC_RUN(execv_missing); TC_RUN(wexecl_missing); TC_RUN(execlp_bare_missing);
    return tc_end();
}
