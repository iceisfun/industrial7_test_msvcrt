/* TC
 * id:    io_neg_lseek_overflow32
 * area:  lowio
 * funcs: _lseek _lseeki64 _tell _open _close
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _tell: returns -1L on error
 * note:  PROBE: result of the 32-bit _tell/_lseek when the position is >= 2^31 (not representable in long).
 *        Assumed: ucrt returns -1 (errno EOVERFLOW); older CRTs return a negative long (-1 or the wrapped value). Whole file is a probe.
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

static const char *volatile fname = "tc_io_neg_lseek_overflow32_" TC_STR(TC_CRT_VER) ".tmp";
static int mk(void) { return _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE); }
TC_CASE(tell_2g)
{
    long r; int fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_lseeki64(fd, 0x80000000LL, SEEK_SET), 0x80000000LL);
    errno = 0; r = _tell(fd);
#ifdef TC_UCRT
    CHECK_EQ_I(r, -1);
#else
    CHECK(r < 0);
#endif
    _close(fd);
}
TC_CASE(lseek_2g)
{
    long r; int fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    errno = 0; r = _lseek(fd, 0x7fffffff, SEEK_SET); CHECK_EQ_I(r, 0x7fffffff);
    errno = 0; r = _lseek(fd, 1, SEEK_CUR);
#ifdef TC_UCRT
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EOVERFLOW);
#else
    CHECK(r < 0);
#endif
    _close(fd);
}
int main(void)
{
    tc_begin();
    TC_RUN(tell_2g); TC_RUN(lseek_2g);
    _unlink(fname);
    return tc_end();
}
