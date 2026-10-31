/* TC
 * id:    io_neg_write_rdonly
 * area:  lowio
 * funcs: _write _open _close _filelength
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _write (returns -1, errno EBADF if fd invalid or not open for writing)
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_io_neg_write_rdonly_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(write_rdonly)
{
    int fd, r;
    fd = _open(fn, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abc", 3), 3);
    CHECK_EQ_I(_close(fd), 0);
    fd = _open(fn, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0);
    if (fd < 0) { _unlink(fn); return; }
    errno = 0;
    r = _write(fd, "xyz", 3);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(_filelength(fd), 3);
    CHECK_EQ_I(_close(fd), 0);
    _unlink(fn);
}
int main(void) { tc_begin(); if (TC_AT_LEAST(80)) tc_iph_install(); TC_RUN(write_rdonly); return tc_end(); }
