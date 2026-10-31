/* TC
 * id:    io_tell_basic
 * area:  lowio
 * funcs: _tell _telli64 _open _write _read _close
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _tell/_telli64
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

static const char *volatile fname = "tc_io_tell_basic_" TC_STR(TC_CRT_VER) ".tmp";
TC_CASE(tracks)
{
    char b[128];
    int fd = _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_tell(fd), 0);
    CHECK_EQ_I(_write(fd, "abcdefghij", 10), 10);
    CHECK_EQ_I(_tell(fd), 10);
    CHECK_EQ_I(_telli64(fd), 10);
    CHECK_EQ_I(_lseek(fd, 2, SEEK_SET), 2);
    CHECK_EQ_I(_read(fd, b, 3), 3);
    CHECK_EQ_I(_tell(fd), 5);
    CHECK_EQ_I(_telli64(fd), _tell(fd));
    CHECK_EQ_I(_read(fd, b, 100), 5);
    CHECK_EQ_I(_tell(fd), 10);
    _close(fd);
}
TC_CASE(append_mode)
{
    int fd = _open(fname, _O_WRONLY | _O_APPEND | _O_BINARY);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "xyz", 3), 3);
    CHECK_EQ_I(_tell(fd), 13);
    CHECK_EQ_I(_telli64(fd), 13);
    _close(fd);
}
int main(void)
{
    tc_begin();
    TC_RUN(tracks); TC_RUN(append_mode);
    _unlink(fname);
    return tc_end();
}
