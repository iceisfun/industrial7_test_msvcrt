/* TC
 * id:    io_neg_locking_invalid
 * area:  lowio
 * funcs: _locking _open _close _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _locking (EBADF invalid file descriptor; EINVAL invalid argument given for mode); Parameter validation
 * note:  Negative nbytes is probed in io_neg_locking_negbytes_probe; unlocking a never-locked region in io_neg_locking_unlock_probe. Invalid-handler count is asserted only for the bad fd.
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/locking.h>

static const char *volatile fn = "tc_io_neg_locking_inv_" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(bad_mode)
{
    volatile int mode = 99; int fd, r;
    DeleteFileA(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    CHECK(tc_iph_install());
    errno = 0;
    r = _locking(fd, mode, 10);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    if (fd >= 0) _close(fd);
    DeleteFileA(fn);
}
TC_CASE(bad_fd)
{
    volatile int fd = -1; int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _locking(fd, _LK_NBLCK, 10);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(bad_mode); TC_RUN(bad_fd); return tc_end(); }
