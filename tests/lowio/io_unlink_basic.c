/* TC
 * id:    io_unlink_basic
 * area:  lowio
 * funcs: _unlink _wunlink _open _close _access
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _unlink, _wunlink ("Each of these functions returns 0 if successful"); _access
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_unlink_basic_" TC_S(TC_CRT_VER) ".tmp";
static const wchar_t *volatile wfn = L"tc_io_unlink_basic_w_" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(unlink_closed)
{
    int fd, r;
    DeleteFileA(fn);
    fd = _open(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    CHECK_EQ_I(_access(fn, 0), 0);
    if (fd >= 0) CHECK_EQ_I(_close(fd), 0);
    r = _unlink(fn);
    CHECK_EQ_I(r, 0);
    errno = 0;
    CHECK_EQ_I(_access(fn, 0), -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK(GetFileAttributesA(fn) == INVALID_FILE_ATTRIBUTES);
}
TC_CASE(wunlink_closed)
{
    HANDLE h; int r;
    DeleteFileW(wfn);
    h = CreateFileW(wfn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    r = _wunlink(wfn);
    CHECK_EQ_I(r, 0);
    CHECK(GetFileAttributesW(wfn) == INVALID_FILE_ATTRIBUTES);
}
TC_CASE(unlink_with_content)
{
    HANDLE h; DWORD w; int r;
    DeleteFileA(fn);
    h = CreateFileA(fn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, "hello", 5, &w, NULL); CloseHandle(h); }
    r = _unlink(fn);
    CHECK_EQ_I(r, 0);
    CHECK(GetFileAttributesA(fn) == INVALID_FILE_ATTRIBUTES);
}
int main(void) { tc_begin(); TC_RUN(unlink_closed); TC_RUN(wunlink_closed); TC_RUN(unlink_with_content); return tc_end(); }
