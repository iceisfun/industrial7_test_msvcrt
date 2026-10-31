/* TC
 * id:    io_neg_close_invalid
 * area:  lowio
 * funcs: _close _open _write
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _close: invalid fd -> invalid parameter handler, EBADF, returns -1
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static char N[] = "tc_io_neg_close_invalid.tmp";

TC_CASE(close_minus_one)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_close(-1), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(close_high_unopened)
{
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_close(5000), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(double_close)
{
    int a, b; _unlink(N);
    tc_iph_install();
    a = _open(N, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    b = _open(N, _O_RDWR | _O_BINARY);
    CHECK(a >= 0 && b >= 0);
    if (a < 0 || b < 0) { if (a >= 0) _close(a); if (b >= 0) _close(b); _unlink(N); return; }
    CHECK_EQ_I(_close(a), 0);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_close(a), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
    /* other descriptor unaffected */
    CHECK_EQ_I(_write(b, "z", 1), 1);
    CHECK_EQ_I(_close(b), 0);
    _unlink(N);
}

int main(void) { tc_begin(); TC_RUN(close_minus_one); TC_RUN(close_high_unopened); TC_RUN(double_close); return tc_end(); }
