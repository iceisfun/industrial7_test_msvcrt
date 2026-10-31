/* TC
 * id:    io_chsize_basic
 * area:  lowio
 * funcs: _chsize _open _write _filelength _read _close _lseek
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _chsize: extends with null characters or truncates; returns 0 on success
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void mk(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
#define N "tc_io_chsize_basic_1.tmp"
static int mkfd(void) { rm(N); return _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE); }
TC_CASE(shrink)
{
    int fd = mkfd(); char b[16] = {0};
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abcdefgh", 8), 8);
    CHECK_EQ_I(_chsize(fd, 3), 0);
    CHECK_EQ_I(_filelength(fd), 3);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(fd, b, 16), 3);
    CHECK_MEM(b, "abc", 3);
    _close(fd); rm(N);
}
TC_CASE(extend)
{
    int fd = mkfd(); char b[16]; static const char want[10] = {'a','b','c',0,0,0,0,0,0,0};
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abc", 3), 3);
    CHECK_EQ_I(_chsize(fd, 10), 0);
    CHECK_EQ_I(_filelength(fd), 10);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(fd, b, 16), 10);
    CHECK_MEM(b, want, 10);
    _close(fd); rm(N);
}
TC_CASE(zero)
{
    int fd = mkfd(); char b[4];
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abc", 3), 3);
    CHECK_EQ_I(_chsize(fd, 0), 0);
    CHECK_EQ_I(_filelength(fd), 0);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(fd, b, 4), 0);
    _close(fd); rm(N);
}
TC_CASE(same_size)
{
    int fd = mkfd();
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abc", 3), 3);
    CHECK_EQ_I(_chsize(fd, 3), 0);
    CHECK_EQ_I(_filelength(fd), 3);
    _close(fd); rm(N);
}
int main(void) { tc_begin(); TC_RUN(shrink); TC_RUN(extend); TC_RUN(zero); TC_RUN(same_size); return tc_end(); }
