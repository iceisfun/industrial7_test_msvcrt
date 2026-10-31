/* TC
 * id:    io_neg_lseek_badwhence
 * area:  lowio
 * funcs: _lseek _lseeki64
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _lseek/_lseeki64: invalid origin -> EINVAL, -1
 * note:  handler invocation is asserted separately in io_neg_lseek_badwhence_iph_probe.
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

static const char *volatile fname = "tc_io_neg_lseek_badwhence_" TC_STR(TC_CRT_VER) ".tmp";
static int mk(void)
{
    int fd = _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _write(fd, "0123456789", 10);
    return fd;
}
TC_CASE(whence3)
{
    int fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    errno = 0;
    CHECK_EQ_I(_lseek(fd, 0, 3), -1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(_tell(fd), 10);
    _close(fd);
}
TC_CASE(whence_minus1)
{
    int fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    errno = 0;
    CHECK_EQ_I(_lseek(fd, 0, -1), -1); CHECK_EQ_I(errno, EINVAL);
    _close(fd);
}
TC_CASE(i64_variants)
{
    int fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    errno = 0;
    CHECK_EQ_I(_lseeki64(fd, 0, 3), -1); CHECK_EQ_I(errno, EINVAL);
    errno = 0;
    CHECK_EQ_I(_lseeki64(fd, 0, -1), -1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(_telli64(fd), 10);
    _close(fd);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(whence3); TC_RUN(whence_minus1); TC_RUN(i64_variants);
    _unlink(fname);
    return tc_end();
}
