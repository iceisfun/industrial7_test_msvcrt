/* TC
 * id:    io_neg_fd_exhaustion
 * area:  lowio
 * funcs: _open _close
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _setmaxstdio (max 8192 handles); _open: EMFILE when no more descriptors
 * note:  Default limit is 2048 (msvcr/msvcrt) or 8192 (ucrt); assert only EMFILE and bound 8192.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static int fds[9000];
static char nul[] = "NUL";

TC_CASE(exhaust)
{
    int n = 0, i, r;
    tc_iph_install();
    for (;;) {
        if (n >= 9000) break;
        r = _open(nul, _O_RDWR | _O_BINARY);
        if (r < 0) break;
        fds[n++] = r;
    }
    CHECK(n > 0);
    CHECK(n <= 8192);
    CHECK(n < 9000);
    errno = 0;
    r = _open(nul, _O_RDWR | _O_BINARY);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EMFILE);
    if (r >= 0) _close(r);
    if (n > 0) {
        CHECK_EQ_I(_close(fds[n - 1]), 0);
        n--;
        r = _open(nul, _O_RDWR | _O_BINARY);
        CHECK(r >= 0);
        if (r >= 0) fds[n++] = r;
        errno = 0;
        r = _open(nul, _O_RDWR | _O_BINARY);
        CHECK_EQ_I(r, -1);
        if (r >= 0) _close(r);
    }
    for (i = 0; i < n; i++) _close(fds[i]);
    r = _open(nul, _O_RDWR | _O_BINARY);
    CHECK(r >= 0);
    if (r >= 0) _close(r);
}

int main(void) { tc_begin(); TC_RUN(exhaust); return tc_end(); }
