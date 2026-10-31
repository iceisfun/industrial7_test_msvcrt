/* TC
 * id:    io_neg_lseek_negative
 * area:  lowio
 * funcs: _lseek _open _close
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _lseek: EINVAL if the file pointer would be positioned before the beginning of the file
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

static const char *volatile fname = "tc_io_neg_lseek_negative_" TC_STR(TC_CRT_VER) ".tmp";
static int mk(void)
{
    int fd = _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _write(fd, "0123456789", 10);
    return fd;
}
TC_CASE(set_minus1)
{
    int fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    errno = 0; CHECK_EQ_I(_lseek(fd, -1, SEEK_SET), -1); CHECK_EQ_I(errno, EINVAL);
    _close(fd);
}
TC_CASE(cur_minus1_at_0)
{
    int fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    errno = 0; CHECK_EQ_I(_lseek(fd, -1, SEEK_CUR), -1); CHECK_EQ_I(errno, EINVAL);
    _close(fd);
}
TC_CASE(end_before_start)
{
    int fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    errno = 0; CHECK_EQ_I(_lseek(fd, -11, SEEK_END), -1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(_lseek(fd, -10, SEEK_END), 0);   /* exactly the start is valid */
    _close(fd);
}
TC_CASE(position_unchanged)
{
    int fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_lseek(fd, 4, SEEK_SET), 4);
    CHECK_EQ_I(_lseek(fd, -100, SEEK_CUR), -1);
    CHECK_EQ_I(_tell(fd), 4);
    CHECK_EQ_I(_lseek(fd, -1, SEEK_SET), -1);
    CHECK_EQ_I(_tell(fd), 4);
    _close(fd);
}
int main(void)
{
    tc_begin();
    TC_RUN(set_minus1); TC_RUN(cur_minus1_at_0); TC_RUN(end_before_start); TC_RUN(position_unchanged);
    _unlink(fname);
    return tc_end();
}
