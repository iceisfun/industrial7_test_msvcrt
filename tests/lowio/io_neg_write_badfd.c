/* TC
 * id:    io_neg_write_badfd
 * area:  lowio
 * funcs: _write _open _close
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _write; Parameter Validation
 * note:  only the fd == -1 case asserts the invalid-parameter handler count
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_io_neg_write_badfd_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(fd_minus1)
{
    volatile int fd = -1; int r;
    tc_iph_count = 0; errno = 0;
    r = _write(fd, "abc", 3);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(fd_closed)
{
    int fd, r;
    fd = _open(fn, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_close(fd), 0);
    errno = 0;
    r = _write(fd, "abc", 3);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    _unlink(fn);
}
TC_CASE(fd_9999)
{
    volatile int fd = 9999; int r;
    errno = 0;
    r = _write(fd, "abc", 3);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fd_minus1); TC_RUN(fd_closed); TC_RUN(fd_9999); return tc_end(); }
