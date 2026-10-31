/* TC
 * id:    io_write_basic
 * area:  lowio
 * funcs: _write _open _close _filelength _lseek _read
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _write, _filelength
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
static const char *fn = "tc_io_write_basic_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(write_seq)
{
    char buf[32];
    int fd;
    fd = _open(fn, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "0123456789", 10), 10);
    CHECK_EQ_I(_filelength(fd), 10);
    CHECK_EQ_I(_write(fd, "abc", 0), 0);
    CHECK_EQ_I(_filelength(fd), 10);
    CHECK_EQ_I(_write(fd, "ABCDEFGHIJ", 10), 10);
    CHECK_EQ_I(_filelength(fd), 20);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(fd, buf, 32), 20);
    CHECK_MEM(buf, "0123456789ABCDEFGHIJ", 20);
    CHECK_EQ_I(_close(fd), 0);
    _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(write_seq); return tc_end(); }
