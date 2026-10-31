/* TC
 * id:    gio_eof_after_write_and_seek
 * area:  lowio
 * funcs: _eof _open _write _lseek _chsize _close
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _eof: nonzero (1) if the position is at end of file
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
#define N "tc_gio_eof_after_write_and_seek.tmp"
static int mkf(void)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) CHECK_EQ_I(_write(fd, "abcd", 4), 4);
    return fd;
}
TC_CASE(after_write_at_end)
{
    int fd = mkf(); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_eof(fd), 1);
    _close(fd); rm(N);
}
TC_CASE(after_seek_back)
{
    int fd = mkf(); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_eof(fd), 0);
    CHECK_EQ_I(_lseek(fd, 3, SEEK_SET), 3);
    CHECK_EQ_I(_eof(fd), 0);
    _close(fd); rm(N);
}
TC_CASE(after_seek_to_len)
{
    int fd = mkf(); CHECK(fd >= 0); if (fd < 0) return;
    _lseek(fd, 0, SEEK_SET);
    CHECK_EQ_I(_lseek(fd, 4, SEEK_SET), 4);
    CHECK_EQ_I(_eof(fd), 1);
    _close(fd); rm(N);
}
TC_CASE(after_chsize_grow)
{
    int fd = mkf(); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_chsize(fd, 10), 0);
    CHECK_EQ_I(_lseek(fd, 4, SEEK_SET), 4);
    CHECK_EQ_I(_eof(fd), 0);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_END), 10);
    CHECK_EQ_I(_eof(fd), 1);
    _close(fd); rm(N);
}
int main(void)
{
    tc_begin();
    TC_RUN(after_write_at_end); TC_RUN(after_seek_back); TC_RUN(after_seek_to_len); TC_RUN(after_chsize_grow);
    rm(N);
    return tc_end();
}
