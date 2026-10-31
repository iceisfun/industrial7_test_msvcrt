/* TC
 * id:    io_neg_locking_unlock_probe
 * area:  lowio
 * funcs: _locking _open _close
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _locking (EACCES: locking violation, file already locked or unlocked)
 * note:  PROBE. _locking(fd,_LK_UNLCK,10) on a region that was never locked is believed to fail with -1 and
 *        errno EACCES. The exact errno depends on how ERROR_NOT_LOCKED is mapped by the CRT, which is the guess.
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

TC_CASE(unlock_never_locked)
{
    const char *volatile fn = "tc_io_neg_locking_unl_" TC_S(TC_CRT_VER) ".tmp"; int fd, r;
    DeleteFileA(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    errno = 0;
    r = _locking(fd, _LK_UNLCK, 10);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EACCES);
    if (fd >= 0) _close(fd);
    DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(unlock_never_locked); return tc_end(); }
