/* TC
 * id:    io_neg_dup_badfd
 * area:  lowio
 * funcs: _dup _dup2 _open _close
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _dup, _dup2: EBADF for invalid descriptor; invalid parameter handler invoked
 * note:  only -1 is asserted for the 99999 target: EBADF or EMFILE depending on version
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
#define N "tc_io_neg_dup_badfd_1.tmp"
static int openv(void) { rm(N); return _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE); }
TC_CASE(dup_minus1)
{
    int r, before;
    if (!tc_iph_install()) TC_SKIP("no iph");
    before = tc_iph_count; errno = 0;
    r = _dup(-1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    CHECK(tc_iph_count > before);
}
TC_CASE(dup_closed)
{
    int fd = openv(), r;
    tc_iph_install();
    CHECK(fd >= 3);
    if (fd < 0) return;
    _close(fd);
    errno = 0; r = _dup(fd);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    if (r >= 0) _close(r);
    rm(N);
}
TC_CASE(dup2_bad_target)
{
    int fd = openv(), r;
    tc_iph_install();
    CHECK(fd >= 3);
    if (fd < 0) return;
    errno = 0; r = _dup2(fd, -1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    r = _dup2(fd, 99999);
    CHECK_EQ_I(r, -1);
    _close(fd); rm(N);
}
TC_CASE(dup2_bad_source)
{
    int fd = openv(), r;
    tc_iph_install();
    CHECK(fd >= 3);
    if (fd < 0) return;
    errno = 0; r = _dup2(-1, fd);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    _close(fd); rm(N);
}
int main(void) { tc_begin(); TC_RUN(dup_minus1); TC_RUN(dup_closed); TC_RUN(dup2_bad_target); TC_RUN(dup2_bad_source); return tc_end(); }
