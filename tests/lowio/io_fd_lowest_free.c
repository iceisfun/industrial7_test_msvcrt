/* TC
 * id:    io_fd_lowest_free
 * area:  lowio
 * funcs: _open _close
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _open (returns a file descriptor); lowest-free allocation is long-standing CRT behaviour
 * note:  Not stated by MS docs; the CRT scans its fd table from index 0.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static char N[] = "tc_io_fd_lowest_free.tmp";

TC_CASE(reuse_order)
{
    int a, b, c, d, e, f; _unlink(N);
    a = _open(N, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(a >= 0); if (a < 0) return;
    b = _open(N, _O_RDWR | _O_BINARY);
    c = _open(N, _O_RDWR | _O_BINARY);
    CHECK(b > a); CHECK(c > b);
    CHECK_EQ_I(_close(b), 0);
    d = _open(N, _O_RDWR | _O_BINARY);
    CHECK_EQ_I(d, b);
    CHECK_EQ_I(_close(a), 0);
    e = _open(N, _O_RDWR | _O_BINARY);
    CHECK_EQ_I(e, a);
    _close(c); _close(d); _close(e);
    f = _open(N, _O_RDWR | _O_BINARY);
    CHECK_EQ_I(f, a);
    _close(f);
    _unlink(N);
}

int main(void) { tc_begin(); TC_RUN(reuse_order); return tc_end(); }
