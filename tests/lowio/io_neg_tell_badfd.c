/* TC
 * id:    io_neg_tell_badfd
 * area:  lowio
 * funcs: _tell _telli64 _open _close
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _tell, _telli64: returns -1L and sets errno to EBADF for an invalid file descriptor; invalid parameter handler invoked
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
#define N "tc_io_neg_tell_badfd_1.tmp"
static int ropen(void) { rm(N); return _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE); }
TC_CASE(tell_minus1) { int c = tc_iph_count; volatile int fd = -1; errno = 0; CHECK_EQ_I(_tell(fd), -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, c + 1); }
TC_CASE(tell_closed)
{
    int fd = ropen(); volatile int v;
    CHECK(fd >= 3); if (fd < 0) return;
    _close(fd); v = fd; errno = 0;
    CHECK_EQ_I(_tell(v), -1); CHECK_EQ_I(errno, EBADF);
    rm(N);
}
TC_CASE(tell_huge) { volatile int fd = 9999; int c = tc_iph_count; errno = 0; CHECK_EQ_I(_tell(fd), -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, c + 1); }
TC_CASE(telli64_minus1) { int c = tc_iph_count; volatile int fd = -1; errno = 0; CHECK(_telli64(fd) == -1LL); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, c + 1); }
TC_CASE(telli64_closed)
{
    int fd = ropen(); volatile int v;
    CHECK(fd >= 3); if (fd < 0) return;
    _close(fd); v = fd; errno = 0;
    CHECK(_telli64(v) == -1LL); CHECK_EQ_I(errno, EBADF);
    rm(N);
}
TC_CASE(telli64_ok_after_failure)
{
    int fd = ropen(); volatile int bad = -1;
    CHECK(fd >= 3); if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abcdefg", 7), 7);
    _tell(bad); _telli64(bad);
    CHECK_EQ_I(_tell(fd), 7); CHECK(_telli64(fd) == 7LL);
    _close(fd); rm(N);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(tell_minus1); TC_RUN(tell_closed); TC_RUN(tell_huge);
    TC_RUN(telli64_minus1); TC_RUN(telli64_closed); TC_RUN(telli64_ok_after_failure);
    rm(N); return tc_end();
}
