/* TC
 * id:    io_osfhandle_nul_pipe_kinds
 * area:  lowio
 * funcs: _open _pipe _close _get_osfhandle _open_osfhandle _isatty _fstat64
 * crt:   ucrtbase msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _get_osfhandle, _open_osfhandle, _isatty, _fstat
 * note:  Observed on Windows 10 22H2 (ucrtbase and msvcrt.dll alike). The OS handle behind a descriptor is
 *        classified by GetFileType: a NUL descriptor is FILE_TYPE_CHAR (not a console: GetConsoleMode fails with
 *        ERROR_INVALID_HANDLE), a _pipe descriptor is FILE_TYPE_PIPE. _isatty is nonzero for NUL and 0 for pipes;
 *        _fstat64 reports _S_IFCHR (st_rdev == st_dev) and _S_IFIFO. _open_osfhandle refuses a handle that
 *        GetFileType cannot classify (a mailslot: EINVAL) and the pseudo handle -2 (EBADF).
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

TC_CASE(nul_fd_handle_is_char_device)
{
    struct _stat64 st; DWORD m = 0; HANDLE h; int fd = _open("NUL", _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    if (fd < 0) return;
    h = (HANDLE)_get_osfhandle(fd);
    CHECK(h != INVALID_HANDLE_VALUE && h != NULL);
    CHECK_EQ_I(GetFileType(h), FILE_TYPE_CHAR);
    SetLastError(0); CHECK_EQ_I(GetConsoleMode(h, &m), 0); CHECK_EQ_I(GetLastError(), ERROR_INVALID_HANDLE);
    CHECK(_isatty(fd) != 0);
    memset(&st, 0, sizeof st);
    CHECK_EQ_I(_fstat64(fd, &st), 0);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFCHR);
    CHECK_EQ_I(st.st_rdev, st.st_dev);
    _close(fd);
}
TC_CASE(nul_fd_io_through_crt_handle)
{
    char b[4]; DWORD n = 0x55; HANDLE h; int fd = _open("NUL", _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_read(fd, b, sizeof b), 0);                    /* NUL reads as an empty stream */
    CHECK_EQ_I(_write(fd, "abcd", 4), 4);                     /* and swallows writes in full */
    h = (HANDLE)_get_osfhandle(fd);
    CHECK(WriteFile(h, "ef", 2, &n, NULL)); CHECK_EQ_I(n, 2);
    _close(fd);
}
TC_CASE(pipe_fd_handles_are_pipes)
{
    struct _stat64 st; int f[2] = { -1, -1 }; HANDLE r, w; DWORD m = 0;
    CHECK_EQ_I(_pipe(f, 256, _O_BINARY), 0);
    if (f[0] < 0) return;
    r = (HANDLE)_get_osfhandle(f[0]); w = (HANDLE)_get_osfhandle(f[1]);
    CHECK(r != INVALID_HANDLE_VALUE && w != INVALID_HANDLE_VALUE && r != w);
    CHECK_EQ_I(GetFileType(r), FILE_TYPE_PIPE); CHECK_EQ_I(GetFileType(w), FILE_TYPE_PIPE);
    SetLastError(0); CHECK_EQ_I(GetConsoleMode(r, &m), 0); CHECK_EQ_I(GetLastError(), ERROR_INVALID_HANDLE);
    CHECK_EQ_I(_isatty(f[0]), 0); CHECK_EQ_I(_isatty(f[1]), 0);
    memset(&st, 0, sizeof st);
    CHECK_EQ_I(_fstat64(f[0], &st), 0);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFIFO);
    CHECK_EQ_I(st.st_nlink, 1);
    CHECK_EQ_I(_write(f[1], "xyz", 3), 3);
    { char b[8] = { 0 }; CHECK_EQ_I(_read(f[0], b, sizeof b), 3); CHECK_MEM(b, "xyz", 3); }
    _close(f[0]); _close(f[1]);
}
TC_CASE(pipe_handle_adopted_by_open_osfhandle)
{
    HANDLE r = NULL, w = NULL; SECURITY_ATTRIBUTES sa = { sizeof sa, NULL, FALSE }; int fr, fw; struct _stat64 st;
    CHECK(CreatePipe(&r, &w, &sa, 0));
    fr = _open_osfhandle((intptr_t)r, _O_RDONLY | _O_BINARY);
    fw = _open_osfhandle((intptr_t)w, _O_WRONLY | _O_BINARY);
    CHECK(fr >= 3 && fw >= 3);
    if (fr >= 0 && fw >= 0) {
        CHECK_EQ_I(_isatty(fr), 0);
        memset(&st, 0, sizeof st);
        CHECK_EQ_I(_fstat64(fr, &st), 0); CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFIFO);
        CHECK_EQ_I(_get_osfhandle(fr), (intptr_t)r);
        CHECK_EQ_I(_write(fw, "q", 1), 1);
    }
    if (fr >= 0) _close(fr); else CloseHandle(r);
    if (fw >= 0) _close(fw); else CloseHandle(w);
}
TC_CASE(nul_handle_adopted_by_open_osfhandle)
{
    HANDLE h = CreateFileA("NUL", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    int fd; struct _stat64 st;
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h == INVALID_HANDLE_VALUE) return;
    fd = _open_osfhandle((intptr_t)h, _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    if (fd < 0) { CloseHandle(h); return; }
    CHECK(_isatty(fd) != 0);
    memset(&st, 0, sizeof st);
    CHECK_EQ_I(_fstat64(fd, &st), 0); CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFCHR);
    CHECK_EQ_I(_get_osfhandle(fd), (intptr_t)h);
    _close(fd);
}
TC_CASE(neg_open_osfhandle_mailslot_einval)
{
    char name[64]; HANDLE s; int fd;
    sprintf(name, "\\\\.\\mailslot\\tc_io_osfh_%lu", (unsigned long)GetCurrentProcessId());
    s = CreateMailslotA(name, 0, 0, NULL);
    CHECK(s != INVALID_HANDLE_VALUE);
    if (s == INVALID_HANDLE_VALUE) return;
    CHECK_EQ_I(GetFileType(s), FILE_TYPE_UNKNOWN);
    tc_iph_install(); errno = 0;
    fd = _open_osfhandle((intptr_t)s, _O_RDONLY | _O_BINARY);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(errno, EINVAL);
    if (fd >= 0) _close(fd); else CloseHandle(s);
}
TC_CASE(neg_open_osfhandle_pseudo_minus2)
{
    int fd;
    tc_iph_install(); errno = 0;
    fd = _open_osfhandle((intptr_t)-2, _O_RDONLY | _O_BINARY);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(errno, EBADF);
    if (fd >= 0) _close(fd);
}

int main(void)
{
    tc_begin();
    TC_RUN(nul_fd_handle_is_char_device);
    TC_RUN(nul_fd_io_through_crt_handle);
    TC_RUN(pipe_fd_handles_are_pipes);
    TC_RUN(pipe_handle_adopted_by_open_osfhandle);
    TC_RUN(nul_handle_adopted_by_open_osfhandle);
    TC_RUN(neg_open_osfhandle_mailslot_einval);
    TC_RUN(neg_open_osfhandle_pseudo_minus2);
    return tc_end();
}
