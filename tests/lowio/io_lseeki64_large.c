/* TC
 * id:    io_lseeki64_large
 * area:  lowio
 * funcs: _lseeki64 _telli64 _open _close
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _lseeki64/_telli64 (64-bit offsets)
 * note:  setting the file pointer beyond EOF allocates nothing; nothing is written.
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

static const char *volatile fname = "tc_io_lseeki64_large_" TC_STR(TC_CRT_VER) ".tmp";
static int mk(void) { return _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE); }
TC_CASE(seek_4g)
{
    int fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_lseeki64(fd, 0x100000000LL, SEEK_SET), 0x100000000LL);
    CHECK_EQ_I(_telli64(fd), 0x100000000LL);
    CHECK_EQ_I(_lseeki64(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_telli64(fd), 0);
    _close(fd);
}
TC_CASE(seek_cur_relative)
{
    int fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_lseeki64(fd, 0x100000000LL, SEEK_SET), 0x100000000LL);
    CHECK_EQ_I(_lseeki64(fd, 5, SEEK_CUR), 0x100000005LL);
    CHECK_EQ_I(_lseeki64(fd, -0x100000000LL, SEEK_CUR), 5);
    _close(fd);
}
int main(void)
{
    tc_begin();
    TC_RUN(seek_4g); TC_RUN(seek_cur_relative);
    _unlink(fname);
    return tc_end();
}
