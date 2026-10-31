/* TC
 * id:    io_neg_close_old_noiph
 * area:  lowio
 * funcs: _close _open
 * crt:   msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _close
 * note:  msvcrt.dll predates the invalid parameter handler in some builds; no handler assertion
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static char N[] = "tc_io_neg_close_old_noiph.tmp";

TC_CASE(close_minus_one)
{
    tc_iph_install();
    errno = 0;
    CHECK_EQ_I(_close(-1), -1);
    CHECK_EQ_I(errno, EBADF);
}
TC_CASE(double_close)
{
    int fd; _unlink(N);
    tc_iph_install();
    fd = _open(N, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_close(fd), 0);
    errno = 0;
    CHECK_EQ_I(_close(fd), -1);
    CHECK_EQ_I(errno, EBADF);
    _unlink(N);
}

int main(void) { tc_begin(); TC_RUN(close_minus_one); TC_RUN(double_close); return tc_end(); }
