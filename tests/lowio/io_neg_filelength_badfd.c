/* TC
 * id:    io_neg_filelength_badfd
 * area:  lowio
 * funcs: _filelength _filelengthi64 _open _close
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _filelength: EBADF for an invalid descriptor, invalid parameter handler invoked
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

static const char *volatile fname = "tc_io_neg_filelength_badfd_" TC_STR(TC_CRT_VER) ".tmp";
static int closed_fd(void)
{
    int fd = _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _close(fd);
    return fd;
}
TC_CASE(m1) { int c = tc_iph_count; errno = 0; CHECK_EQ_I(_filelength(-1), -1); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count > c); }
TC_CASE(closed) { int fd = closed_fd(), c = tc_iph_count; CHECK(fd >= 0); errno = 0; CHECK_EQ_I(_filelength(fd), -1); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count > c); }
TC_CASE(i64_m1) { int c = tc_iph_count; errno = 0; CHECK_EQ_I(_filelengthi64(-1), -1); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count > c); }
TC_CASE(i64_closed) { int fd = closed_fd(), c = tc_iph_count; CHECK(fd >= 0); errno = 0; CHECK_EQ_I(_filelengthi64(fd), -1); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count > c); }
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(m1); TC_RUN(closed); TC_RUN(i64_m1); TC_RUN(i64_closed);
    _unlink(fname);
    return tc_end();
}
