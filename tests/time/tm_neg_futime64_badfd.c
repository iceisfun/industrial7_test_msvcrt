/* TC
 * id:    tm_neg_futime64_badfd
 * area:  time
 * funcs: _futime64 _open _close
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _futime: invalid descriptor -> invalid parameter handler, errno EBADF
 */
#include "tc.h"
#include <sys/utime.h>
#include <time.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

static struct __utimbuf64 B = { 1000000000, 1100000000 };
static const char *F = "tc_tm_neg_futime64_badfd_a.tmp";

TC_CASE(fd_minus1)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_futime64(-1, &B), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(closed_fd)
{
    CHECK(tc_iph_install());
    int fd = _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_close(fd), 0);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_futime64(fd, &B), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(huge_fd)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_futime64(9999, &B), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(fd_minus1); TC_RUN(closed_fd); TC_RUN(huge_fd); DeleteFileA(F); return tc_end(); }
