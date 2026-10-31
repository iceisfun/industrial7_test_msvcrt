/* TC
 * id:    prc_neg_spawnl_not_executable
 * area:  process
 * funcs: _spawnl _wspawnl
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _spawn: ENOEXEC = file is not executable or has an invalid executable-file format
 * note:  File is named .exe so the CRT hands it straight to CreateProcess (ERROR_BAD_EXE_FORMAT -> ENOEXEC) regardless of extension handling.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define FN "tc_prc_notexe_" TC_S(TC_CRT_VER) ".exe"

static int del_retry(const char *fn)
{
    int i; for (i = 0; i < 40; i++) { if (DeleteFileA(fn) || GetFileAttributesA(fn) == INVALID_FILE_ATTRIBUTES) return 1; Sleep(50); }
    return 0;
}

static int make_file(void)
{
    HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD n; const char *t = "this is plain text, not a PE image\r\nline two\r\n";
    if (h == INVALID_HANDLE_VALUE) return 0;
    WriteFile(h, t, 46, &n, NULL); CloseHandle(h); return 1;
}

TC_CASE(spawnl_text_file)
{
    volatile int m = _P_WAIT; intptr_t r;
    CHECK(make_file());
    errno = 0;
    r = _spawnl(m, FN, FN, NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOEXEC);
    del_retry(FN);
}
TC_CASE(wspawnl_text_file)
{
    volatile int m = _P_WAIT; intptr_t r;
    CHECK(make_file());
    errno = 0;
    r = _wspawnl(m, L"" FN, L"" FN, NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOEXEC);
    del_retry(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(spawnl_text_file); TC_RUN(wspawnl_text_file);
    return tc_end();
}
