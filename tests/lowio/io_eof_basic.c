/* TC
 * id:    io_eof_basic
 * area:  lowio
 * funcs: _eof _open _read _write _lseek _close
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _eof: 1 at end of file, 0 otherwise, -1 on error
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

static const char *volatile fname = "tc_io_eof_basic_" TC_STR(TC_CRT_VER) ".tmp";
static int mk(int trunc_only)
{
    int fd = _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0 && !trunc_only) { _write(fd, "0123456789", 10); _lseek(fd, 0, SEEK_SET); }
    return fd;
}
TC_CASE(not_eof_start_and_mid)
{
    char b[16]; int fd = mk(0); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_eof(fd), 0);
    CHECK_EQ_I(_read(fd, b, 4), 4);
    CHECK_EQ_I(_eof(fd), 0);
    _close(fd);
}
TC_CASE(eof_after_read_all)
{
    char b[16]; int fd = mk(0); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_read(fd, b, 10), 10);
    CHECK_EQ_I(_eof(fd), 1);
    _close(fd);
}
TC_CASE(eof_empty) { int fd = mk(1); CHECK(fd >= 0); if (fd < 0) return; CHECK_EQ_I(_eof(fd), 1); _close(fd); }
TC_CASE(eof_after_seek_end)
{
    int fd = mk(0); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_lseek(fd, 0, SEEK_END), 10);
    CHECK_EQ_I(_eof(fd), 1);
    CHECK_EQ_I(_lseek(fd, -1, SEEK_END), 9);
    CHECK_EQ_I(_eof(fd), 0);
    _close(fd);
}
int main(void)
{
    tc_begin();
    TC_RUN(not_eof_start_and_mid); TC_RUN(eof_after_read_all); TC_RUN(eof_empty); TC_RUN(eof_after_seek_end);
    _unlink(fname);
    return tc_end();
}
