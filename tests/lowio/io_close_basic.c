/* TC
 * id:    io_close_basic
 * area:  lowio
 * funcs: _close _open _read _write
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _close, _open, _read
 * note:  fd reuse after close is not asserted here (see io_fd_lowest_free)
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static char N[] = "tc_io_close_basic.tmp";

TC_CASE(close_releases)
{
    int fd, fd2; char b[4];
    tc_iph_install();
    _unlink(N);
    fd = _open(N, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_close(fd), 0);
    errno = 0;
    CHECK_EQ_I(_read(fd, b, 4), -1);
    CHECK_EQ_I(errno, EBADF);
    errno = 0;
    CHECK_EQ_I(_write(fd, "a", 1), -1);
    CHECK_EQ_I(errno, EBADF);
    fd2 = _open(N, _O_RDONLY | _O_BINARY);
    CHECK(fd2 >= 0);
    if (fd2 >= 0) _close(fd2);
    _unlink(N);
}

int main(void) { tc_begin(); TC_RUN(close_releases); return tc_end(); }
