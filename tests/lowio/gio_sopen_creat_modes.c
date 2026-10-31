/* TC
 * id:    gio_sopen_creat_modes
 * area:  lowio
 * funcs: _sopen _close _read _write _lseek _unlink
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _sopen: oflag as for _open (_O_CREAT, _O_TRUNC, _O_APPEND, _O_EXCL, _O_TEMPORARY, _O_TEXT, _O_BINARY)
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER)
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_gio_sopen_creat_modes_" TCV ".tmp";
#define PM (_S_IREAD | _S_IWRITE)

static long rawsize(const char *n)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD s;
    if (h == INVALID_HANDLE_VALUE) return -1;
    s = GetFileSize(h, NULL); CloseHandle(h); return (long)s;
}

TC_CASE(creat_trunc_text)
{
    int fd; _unlink(fn);
    fd = _sopen(fn, _O_CREAT | _O_TRUNC | _O_WRONLY | _O_TEXT, _SH_DENYNO, PM);
    CHECK(fd >= 3);
    if (fd >= 0) { CHECK_EQ_I(_write(fd, "\n", 1), 1); _close(fd); }
    CHECK_EQ_I(rawsize(fn), 2);
    _unlink(fn);
}
TC_CASE(binary_no_translate)
{
    int fd; _unlink(fn);
    fd = _sopen(fn, _O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY, _SH_DENYNO, PM);
    CHECK(fd >= 3);
    if (fd >= 0) { CHECK_EQ_I(_write(fd, "\n", 1), 1); _close(fd); }
    CHECK_EQ_I(rawsize(fn), 1);
    _unlink(fn);
}
TC_CASE(append)
{
    int fd; char b[16] = {0}; _unlink(fn);
    fd = _sopen(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _SH_DENYNO, PM);
    if (fd >= 0) { _write(fd, "ab", 2); _close(fd); }
    fd = _sopen(fn, _O_WRONLY | _O_APPEND | _O_BINARY, _SH_DENYNO);
    CHECK(fd >= 0);
    if (fd >= 0) { _lseek(fd, 0, SEEK_SET); CHECK_EQ_I(_write(fd, "cd", 2), 2); _close(fd); }
    fd = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK(fd >= 0);
    if (fd >= 0) { CHECK_EQ_I(_read(fd, b, 15), 4); _close(fd); }
    CHECK_STR(b, "abcd");
    _unlink(fn);
}
TC_CASE(rdwr_readback)
{
    int fd; char b[16] = {0}; _unlink(fn);
    fd = _sopen(fn, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _SH_DENYNO, PM);
    CHECK(fd >= 0);
    if (fd >= 0) {
        CHECK_EQ_I(_write(fd, "hello", 5), 5);
        CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
        CHECK_EQ_I(_read(fd, b, 15), 5);
        _close(fd);
    }
    CHECK_STR(b, "hello");
    _unlink(fn);
}
TC_CASE(excl_new)
{
    int fd; _unlink(fn);
    fd = _sopen(fn, _O_CREAT | _O_EXCL | _O_WRONLY | _O_BINARY, _SH_DENYNO, PM);
    CHECK(fd >= 3);
    if (fd >= 0) _close(fd);
    CHECK_EQ_I(rawsize(fn), 0);
    _unlink(fn);
}
TC_CASE(temporary_flag)
{
    int fd; _unlink(fn);
    fd = _sopen(fn, _O_CREAT | _O_RDWR | _O_BINARY | _O_TEMPORARY, _SH_DENYNO, PM);
    CHECK(fd >= 3);
    if (fd >= 0) {
        _write(fd, "x", 1);
        CHECK(GetFileAttributesA(fn) != INVALID_FILE_ATTRIBUTES);
        CHECK_EQ_I(_close(fd), 0);
    }
    CHECK(GetFileAttributesA(fn) == INVALID_FILE_ATTRIBUTES);
    _unlink(fn);
}
int main(void)
{
    tc_begin();
    TC_RUN(creat_trunc_text); TC_RUN(binary_no_translate); TC_RUN(append);
    TC_RUN(rdwr_readback); TC_RUN(excl_new); TC_RUN(temporary_flag);
    return tc_end();
}
