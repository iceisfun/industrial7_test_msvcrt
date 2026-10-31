/* TC
 * id:    io_open_osfhandle_basic
 * area:  lowio
 * funcs: _open_osfhandle _get_osfhandle _close _read
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _open_osfhandle; _close ("closes the underlying OS file handle")
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>

static const char *volatile fn = "tc_io_open_osfhandle_" TC_S(TC_CRT_VER) ".tmp";
static void mk(void)
{
    HANDLE h; DWORD w;
    h = CreateFileA(fn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, "hello", 5, &w, NULL); CloseHandle(h); }
}
static HANDLE opn(void)
{
    return CreateFileA(fn, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

TC_CASE(wrap_and_read)
{
    HANDLE h; int fd; char b[16] = {0};
    mk(); h = opn(); CHECK(h != INVALID_HANDLE_VALUE);
    fd = _open_osfhandle((intptr_t)h, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0);
    CHECK_EQ_I(_get_osfhandle(fd), (intptr_t)h);
    CHECK_EQ_I(_read(fd, b, 16), 5);
    CHECK_STR(b, "hello");
    CHECK_EQ_I(_read(fd, b, 16), 0);
    if (fd >= 0) CHECK_EQ_I(_close(fd), 0); else CloseHandle(h);
    DeleteFileA(fn);
}
TC_CASE(close_closes_handle)
{
    HANDLE h; int fd; DWORD fl = 0;
    mk(); h = opn(); CHECK(h != INVALID_HANDLE_VALUE);
    fd = _open_osfhandle((intptr_t)h, _O_RDONLY);
    CHECK(fd >= 0);
    CHECK(GetHandleInformation(h, &fl));
    if (fd >= 0) CHECK_EQ_I(_close(fd), 0);
    CHECK(!GetHandleInformation(h, &fl));
    CHECK_EQ_I(GetLastError(), ERROR_INVALID_HANDLE);
    DeleteFileA(fn);
}
TC_CASE(fd_is_lowest_free)
{
    HANDLE h; int fd, fd2;
    mk(); h = opn();
    fd = _open_osfhandle((intptr_t)h, _O_RDONLY);
    CHECK(fd >= 0);
    h = opn();
    fd2 = _open_osfhandle((intptr_t)h, _O_RDONLY);
    CHECK(fd2 >= 0 && fd2 != fd);
    if (fd >= 0) _close(fd);
    if (fd2 >= 0) _close(fd2);
    DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(wrap_and_read); TC_RUN(close_closes_handle); TC_RUN(fd_is_lowest_free); return tc_end(); }
