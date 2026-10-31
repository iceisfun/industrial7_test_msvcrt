/* TC
 * id:    io_neg_locking_negbytes_probe
 * area:  lowio
 * funcs: _locking _open _close
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _locking (nbytes: number of bytes to lock)
 * note:  PROBE. _locking(fd,_LK_NBLCK,-1) is believed rejected with -1/EINVAL. (If a CRT instead treats nbytes as an
 *        unsigned 0xFFFFFFFF length the call may succeed; the unlock is attempted in that case.)
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

TC_CASE(negative_nbytes)
{
    const char *volatile fn = "tc_io_neg_locking_nb_" TC_S(TC_CRT_VER) ".tmp"; volatile long nb = -1; int fd, r;
    DeleteFileA(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    tc_iph_install();
    errno = 0;
    r = _locking(fd, _LK_NBLCK, nb);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    if (r == 0) { _lseek(fd, 0, SEEK_SET); _locking(fd, _LK_UNLCK, nb); }
    if (fd >= 0) _close(fd);
    DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(negative_nbytes); return tc_end(); }
