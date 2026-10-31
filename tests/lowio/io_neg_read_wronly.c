/* TC
 * id:    io_neg_read_wronly
 * area:  lowio
 * funcs: _read _open _close
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _read (returns -1, errno EBADF if fd invalid or file not open for reading)
 * note:  asserts only -1 and EBADF (CRT maps ReadFile ERROR_ACCESS_DENIED to EBADF); buffer contents on failure not asserted
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_io_neg_read_wronly_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(read_wronly)
{
    char buf[8] = "XXXXXXX";
    int fd, r;
    fd = _open(fn, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abcdef", 6), 6);
    errno = 0;
    r = _read(fd, buf, 4);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(_close(fd), 0);
    _unlink(fn);
}
int main(void) { tc_begin(); if (TC_AT_LEAST(80)) tc_iph_install(); TC_RUN(read_wronly); return tc_end(); }
