/* TC
 * id:    io_neg_telli64_badfd_msvcr
 * area:  lowio
 * funcs: _tell _telli64 _open _close
 * crt:   win7
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _tell, _telli64: -1 and EBADF for invalid fd; parameter validation (invalid parameter handler) since the VS2005 CRT (msvcr80+)
 * note:  msvcrt.dll has no invalid parameter handler: only errno is asserted there; handler count asserted for generation >= 80.
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
#define N "tc_io_neg_telli64_badfd_msvcr_1.tmp"
static int have_iph;
static int ropen(void) { rm(N); return _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE); }
static void chk_iph(int c) { if (have_iph) CHECK_EQ_I(tc_iph_count, c + 1); }
TC_CASE(tell_minus1_ebadf) { int c = tc_iph_count; volatile int fd = -1; errno = 0; CHECK_EQ_I(_tell(fd), -1); CHECK_EQ_I(errno, EBADF); chk_iph(c); }
TC_CASE(tell_closed_ebadf)
{
    int fd = ropen(); volatile int v;
    CHECK(fd >= 3); if (fd < 0) return;
    _close(fd); v = fd; errno = 0;
    CHECK_EQ_I(_tell(v), -1); CHECK_EQ_I(errno, EBADF);
    rm(N);
}
TC_CASE(telli64_minus1_ebadf) { int c = tc_iph_count; volatile int fd = -1; errno = 0; CHECK(_telli64(fd) == -1LL); CHECK_EQ_I(errno, EBADF); chk_iph(c); }
TC_CASE(telli64_closed_ebadf)
{
    int fd = ropen(); volatile int v;
    CHECK(fd >= 3); if (fd < 0) return;
    _close(fd); v = fd; errno = 0;
    CHECK(_telli64(v) == -1LL); CHECK_EQ_I(errno, EBADF);
    rm(N);
}
int main(void)
{
    tc_begin();
    have_iph = TC_AT_LEAST(80);
    if (have_iph) CHECK(tc_iph_install());
    TC_RUN(tell_minus1_ebadf); TC_RUN(tell_closed_ebadf); TC_RUN(telli64_minus1_ebadf); TC_RUN(telli64_closed_ebadf);
    rm(N); return tc_end();
}
