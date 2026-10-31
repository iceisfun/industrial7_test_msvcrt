/* TC
 * id:    io_neg_chsize_s_returns_errno
 * area:  lowio
 * funcs: _chsize_s _chsize _close _open _write _filelengthi64
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _chsize_s: returns 0 on success, otherwise an error code (EBADF, EINVAL, ...); _chsize returns 0 or -1 with errno set
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <share.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void mk(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static void note(const char *k, long long v) { char b[32]; tc_write("TC INFO " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n"); }
#define N "tc_io_neg_chsize_s_returns_errno_1.tmp"
static int ropen(void) { rm(N); return _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE); }
TC_CASE(badfd) { int c = tc_iph_count; volatile int fd = -1; errno = 0; CHECK_EQ_I(_chsize_s(fd, 0), EBADF); CHECK_EQ_I(tc_iph_count, c + 1); }
TC_CASE(closed)
{
    int fd = ropen(); volatile int v;
    CHECK(fd >= 3); if (fd < 0) return;
    _close(fd); v = fd;
    CHECK_EQ_I(_chsize_s(v, 0), EBADF);
    rm(N);
}
TC_CASE(negative_size)
{
    int fd = ropen(); volatile long long sz = -1;
    CHECK(fd >= 3); if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abcde", 5), 5);
    CHECK_EQ_I(_chsize_s(fd, sz), EINVAL);
    CHECK(_filelengthi64(fd) == 5LL);
    _close(fd); rm(N);
}
TC_CASE(extend_write_only)
{
    int fd; rm(N);
    fd = _open(N, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3); if (fd < 0) return;
    CHECK_EQ_I(_chsize_s(fd, 100), 0);
    CHECK(_filelengthi64(fd) == 100LL);
    _close(fd); rm(N);
}
TC_CASE(chsize_closed_sets_errno)
{
    int fd = ropen(); volatile int v;
    CHECK(fd >= 3); if (fd < 0) return;
    _close(fd); v = fd; errno = 0;
    CHECK_EQ_I(_chsize(v, 0), -1); CHECK_EQ_I(errno, EBADF);
    rm(N);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(badfd); TC_RUN(closed); TC_RUN(negative_size); TC_RUN(extend_write_only); TC_RUN(chsize_closed_sets_errno);
    rm(N); return tc_end();
}
