/* TC
 * id:    io_neg_read_badfd
 * area:  lowio
 * funcs: _read _open _close
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _read; Parameter Validation (invalid fd -> EBADF, handler)
 * note:  only the fd == -1 case asserts the invalid-parameter handler count (fd outside range), others assert -1/EBADF only
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_io_neg_read_badfd_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(fd_minus1)
{
    char buf[8]; volatile int fd = -1; int r;
    tc_iph_count = 0; errno = 0;
    r = _read(fd, buf, 4);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(fd_1000)
{
    char buf[8]; volatile int fd = 1000; int r;
    errno = 0;
    r = _read(fd, buf, 4);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
}
TC_CASE(fd_closed)
{
    char buf[8]; int fd, r;
    fd = _open(fn, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_close(fd), 0);
    errno = 0;
    r = _read(fd, buf, 4);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    _unlink(fn);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fd_minus1); TC_RUN(fd_1000); TC_RUN(fd_closed); return tc_end(); }
