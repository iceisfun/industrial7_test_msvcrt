/* TC
 * id:    io_neg_filelengthi64_badfd
 * area:  lowio
 * funcs: _filelengthi64 _filelength _open _close _write _lseek _tell
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _filelength, _filelengthi64: -1 and EBADF for invalid fd; invalid parameter handler invoked
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
#define N "tc_io_neg_filelengthi64_badfd_1.tmp"
static int ropen(void) { rm(N); return _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE); }
TC_CASE(minus1) { int c = tc_iph_count; volatile int fd = -1; errno = 0; CHECK(_filelengthi64(fd) == -1LL); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, c + 1); }
TC_CASE(closed)
{
    int fd = ropen(); volatile int v;
    CHECK(fd >= 3); if (fd < 0) return;
    _close(fd); v = fd; errno = 0;
    CHECK(_filelengthi64(v) == -1LL); CHECK_EQ_I(errno, EBADF);
    rm(N);
}
TC_CASE(fd9999) { volatile int fd = 9999; errno = 0; CHECK(_filelengthi64(fd) == -1LL); CHECK_EQ_I(errno, EBADF); errno = 0; CHECK_EQ_I(_filelength(fd), -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(valid_after_failure)
{
    int fd = ropen(); volatile int bad = -1;
    CHECK(fd >= 3); if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "0123456789", 10), 10);
    CHECK_EQ_I(_lseek(fd, 4, SEEK_SET), 4);
    _filelengthi64(bad); _filelength(bad);
    CHECK(_filelengthi64(fd) == 10LL); CHECK_EQ_I(_filelength(fd), 10);
    CHECK_EQ_I(_tell(fd), 4);
    _close(fd); rm(N);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(minus1); TC_RUN(closed); TC_RUN(fd9999); TC_RUN(valid_after_failure);
    rm(N); return tc_end();
}
