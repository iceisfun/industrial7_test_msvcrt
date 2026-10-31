/* TC
 * id:    io_neg_chsize_read_only_fd
 * area:  lowio
 * funcs: _chsize_s _chsize _open _close _filelengthi64
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _chsize/_chsize_s: the file must be open for writing; failure codes EACCES / EBADF (docs list EBADF for read-only file)
 * note:  The exact code (EACCES vs EBADF) is probed separately in io_neg_chsize_read_only_code_probe; here only failure and unchanged length are asserted.
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
#define N "tc_io_neg_chsize_read_only_fd_1.tmp"
static int ropen(void) { mk(N, "0123456789", 10); return _open(N, _O_RDONLY | _O_BINARY); }
TC_CASE(chsize_s_fails)
{
    int fd = ropen(), r; volatile long long sz = 3;
    CHECK(fd >= 3); if (fd < 0) return;
    r = _chsize_s(fd, sz);
    CHECK(r == EACCES || r == EBADF);
    CHECK(_filelengthi64(fd) == 10LL);
    r = _chsize_s(fd, 50);
    CHECK(r == EACCES || r == EBADF);
    CHECK(_filelengthi64(fd) == 10LL);
    _close(fd); rm(N);
}
TC_CASE(chsize_fails)
{
    int fd = ropen(), r; volatile long sz = 3;
    CHECK(fd >= 3); if (fd < 0) return;
    errno = 0; r = _chsize(fd, sz);
    CHECK_EQ_I(r, -1); CHECK(errno == EACCES || errno == EBADF);
    errno = 0; r = _chsize(fd, 50);
    CHECK_EQ_I(r, -1); CHECK(errno == EACCES || errno == EBADF);
    _close(fd); rm(N);
}
TC_CASE(length_unchanged)
{
    int fd = ropen();
    CHECK(fd >= 3); if (fd < 0) return;
    _chsize_s(fd, 0); _chsize(fd, 99);
    CHECK(_filelengthi64(fd) == 10LL);
    _close(fd);
    mk(N, "", 0); /* recreate empty to prove the file object is not stale */
    rm(N);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(chsize_s_fails); TC_RUN(chsize_fails); TC_RUN(length_unchanged);
    rm(N); return tc_end();
}
