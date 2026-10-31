/* TC
 * id:    io_filelength_basic
 * area:  lowio
 * funcs: _filelength _filelengthi64 _open _write _lseek _tell _close
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _filelength/_filelengthi64
 * note:  preserving the file position across _filelength is real-CRT behaviour but not explicitly documented.
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

static const char *volatile fname = "tc_io_filelength_basic_" TC_STR(TC_CRT_VER) ".tmp";
TC_CASE(grows)
{
    int fd = _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_filelength(fd), 0);
    CHECK_EQ_I(_filelengthi64(fd), 0);
    CHECK_EQ_I(_write(fd, "0123456789", 10), 10);
    CHECK_EQ_I(_filelength(fd), 10);
    CHECK_EQ_I(_write(fd, "abc", 3), 3);
    CHECK_EQ_I(_filelength(fd), 13);
    CHECK_EQ_I(_filelengthi64(fd), 13);
    _close(fd);
}
TC_CASE(position_preserved)
{
    int fd = _open(fname, _O_RDWR | _O_BINARY);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_lseek(fd, 4, SEEK_SET), 4);
    CHECK_EQ_I(_filelength(fd), 13);
    CHECK_EQ_I(_tell(fd), 4);
    CHECK_EQ_I(_filelengthi64(fd), 13);
    CHECK_EQ_I(_tell(fd), 4);
    _close(fd);
}
int main(void)
{
    tc_begin();
    TC_RUN(grows); TC_RUN(position_preserved);
    _unlink(fname);
    return tc_end();
}
