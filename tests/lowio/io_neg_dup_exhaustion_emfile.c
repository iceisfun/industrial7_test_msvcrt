/* TC
 * id:    io_neg_dup_exhaustion_emfile
 * area:  lowio
 * funcs: _dup _close _open
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _dup: errno EMFILE when no more file descriptors are available; _setmaxstdio (max 8192)
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <share.h>
static int fds[9000];
static int n;
static char nul[] = "NUL";
TC_CASE(fill_then_dup_fails)
{
    int r, i; volatile int src;
    for (n = 0; n < 9000; n++) { r = _open(nul, _O_RDWR | _O_BINARY); if (r < 0) break; fds[n] = r; }
    CHECK(n > 3); CHECK(n <= 8192); CHECK(n < 9000);
    if (n < 1) return;
    src = fds[0];
    errno = 0; r = _dup(src);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EMFILE);
    if (r >= 0) _close(r);
    errno = 0; r = _dup(src);        /* repeat: no leak, still fails */
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EMFILE);
    if (r >= 0) _close(r);
    (void)i;
}
TC_CASE(close_one_then_dup_gets_it)
{
    int r, hole; volatile int src;
    if (n < 3) TC_SKIP("table not filled");
    hole = fds[n - 2];
    src = fds[0];
    CHECK_EQ_I(_close(hole), 0); fds[n - 2] = -1;
    r = _dup(src);
    CHECK_EQ_I(r, hole);
    if (r >= 0) fds[n - 2] = r;
}
TC_CASE(close_all)
{
    int i, bad = 0, r;
    for (i = 0; i < n; i++) if (fds[i] >= 0 && _close(fds[i]) != 0) bad++;
    CHECK_EQ_I(bad, 0);
    r = _open(nul, _O_RDWR | _O_BINARY); CHECK(r >= 3);
    if (r >= 0) _close(r);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(fill_then_dup_fails); TC_RUN(close_one_then_dup_gets_it); TC_RUN(close_all);
    return tc_end();
}
