/* TC
 * id:    io_neg_lseek_badfd
 * area:  lowio
 * funcs: _lseek _lseeki64 _tell _telli64 _close _open
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _lseek/_tell: EBADF for an invalid file descriptor, invalid parameter handler invoked
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <winioctl.h>
#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)

static const char *volatile fname = "tc_io_neg_lseek_badfd_" TC_STR(TC_CRT_VER) ".tmp";
static int closed_fd(void)
{
    int fd = _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _close(fd);
    return fd;
}
TC_CASE(lseek_m1)  { int c = tc_iph_count; errno = 0; CHECK_EQ_I(_lseek(-1, 0, SEEK_SET), -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count - c, 1); }
TC_CASE(lseeki64_m1) { int c = tc_iph_count; errno = 0; CHECK_EQ_I(_lseeki64(-1, 0, SEEK_SET), -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count - c, 1); }
TC_CASE(tell_m1)   { int c = tc_iph_count; errno = 0; CHECK_EQ_I(_tell(-1), -1); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count > c); }
TC_CASE(telli64_closed)
{
    int fd = closed_fd(), c = tc_iph_count; CHECK(fd >= 0);
    errno = 0; CHECK_EQ_I(_telli64(fd), -1); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count > c);
}
TC_CASE(lseek_closed)
{
    int fd = closed_fd(), c = tc_iph_count; CHECK(fd >= 0);
    errno = 0; CHECK_EQ_I(_lseek(fd, 0, SEEK_CUR), -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count - c, 1);
}
TC_CASE(huge_fd)
{
    int c = tc_iph_count; errno = 0;
    CHECK_EQ_I(_lseek(1000000, 0, SEEK_SET), -1); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count > c);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(lseek_m1); TC_RUN(lseeki64_m1); TC_RUN(tell_m1); TC_RUN(telli64_closed); TC_RUN(lseek_closed); TC_RUN(huge_fd);
    _unlink(fname);
    return tc_end();
}
