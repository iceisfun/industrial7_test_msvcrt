/* TC
 * id:    prc_neg_execl_not_executable
 * area:  process
 * funcs: _execl _execv _wexecl
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _exec, _wexec functions: errno ENOEXEC = file is not executable or has invalid executable-file format
 * note:  OBSERVED on Win10 22H2 (19045): _execl/_execv/_wexecl on a .exe file holding plain text return -1 with errno EINVAL,
 *        not ENOEXEC: CreateProcess fails with GetLastError 216 (ERROR_EXE_MACHINE_TYPE_MISMATCH), which the CRT maps to EINVAL
 *        (ENOEXEC is only produced for ERROR_BAD_EXE_FORMAT 193 / ERROR_BAD_FORMAT 11). The same 216 appears when the text
 *        starts with "MZ". Depends on what CreateProcess returns for non-PE files on build 19045 [not verified on other
 *        builds]; deterministic here.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

#define FN "tc_prc_neg_execl_not_executable_x.exe"
#define WFN L"tc_prc_neg_execl_not_executable_x.exe"

static void mkfile(void)
{
    DWORD n; HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, "this is not a PE image\n", 23, &n, NULL); CloseHandle(h); }
}

TC_CASE(execl_text)
{
    const char *volatile p = FN; intptr_t r; mkfile(); errno = 0;
    r = _execl(p, p, (char *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);   /* observed: CreateProcess fails with ERROR_EXE_MACHINE_TYPE_MISMATCH (216), which the CRT maps to EINVAL, not ENOEXEC */
    DeleteFileA(FN);
}
TC_CASE(execv_text)
{
    const char *volatile p = FN; const char *av[2]; intptr_t r; mkfile(); av[0] = FN; av[1] = NULL; errno = 0;
    r = _execv(p, av);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);   /* observed: CreateProcess fails with ERROR_EXE_MACHINE_TYPE_MISMATCH (216), which the CRT maps to EINVAL, not ENOEXEC */
    DeleteFileA(FN);
}
TC_CASE(wexecl_text)
{
    const wchar_t *volatile p = WFN; intptr_t r; mkfile(); errno = 0;
    r = _wexecl(p, p, (wchar_t *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);   /* observed: CreateProcess fails with ERROR_EXE_MACHINE_TYPE_MISMATCH (216), which the CRT maps to EINVAL, not ENOEXEC */
    DeleteFileA(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(execl_text); TC_RUN(execv_text); TC_RUN(wexecl_text);
    return tc_end();
}
