/* TC
 * id:    io_neg_commit_badfd
 * area:  lowio
 * funcs: _commit _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _commit (EBADF for invalid fd); Parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void mk(const char *n, int ro)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    if (ro) SetFileAttributesA(n, FILE_ATTRIBUTE_READONLY);
}

static const char *volatile fn = "tc_io_neg_commit_badfd.tmp";
TC_CASE(minus_one)
{
    volatile int fd = -1;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_commit(fd), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(closed_fd)
{
    int fd;
    mk(fn, 0);
    fd = _open(fn, _O_RDWR | _O_BINARY);
    CHECK(fd >= 0);
    if (fd < 0) { rm(fn); return; }
    CHECK_EQ_I(_close(fd), 0);
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_commit(fd), -1);
    CHECK_EQ_I(errno, EBADF);
    rm(fn);
}
TC_CASE(huge_fd)
{
    volatile int fd = 4000;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_commit(fd), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(minus_one); TC_RUN(closed_fd); TC_RUN(huge_fd); return tc_end(); }
