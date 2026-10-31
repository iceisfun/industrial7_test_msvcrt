/* TC
 * id:    io_lseek_basic
 * area:  lowio
 * funcs: _lseek _open _close _write
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _lseek (returns offset from beginning of file)
 */
#include "tc.h"
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_io_lseek_basic_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(whence)
{
    int fd;
    fd = _open(fn, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "0123456789", 10), 10);
    CHECK_EQ_I(_lseek(fd, 3, SEEK_SET), 3);
    CHECK_EQ_I(_lseek(fd, 2, SEEK_CUR), 5);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_CUR), 5);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_END), 10);
    CHECK_EQ_I(_lseek(fd, -2, SEEK_END), 8);
    CHECK_EQ_I(_lseek(fd, -3, SEEK_CUR), 5);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_close(fd), 0);
    _unlink(fn);
}
TC_CASE(past_eof)
{
    int fd;
    fd = _open(fn, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "0123456789", 10), 10);
    CHECK_EQ_I(_lseek(fd, 50, SEEK_SET), 50);
    CHECK_EQ_I(_lseek(fd, 5, SEEK_END), 15);
    CHECK_EQ_I(_close(fd), 0);
    _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(whence); TC_RUN(past_eof); return tc_end(); }
