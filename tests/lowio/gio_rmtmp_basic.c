/* TC
 * id:    gio_rmtmp_basic
 * area:  lowio
 * funcs: _rmtmp tmpfile fclose _fileno _eof
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _rmtmp: closes and deletes temporary files created by tmpfile; returns the number closed
 * note:  Cases skip when tmpfile() returns NULL (e.g. root dir not writable on old Windows). Gap vs fio_rmtmp_count.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

TC_CASE(zero_when_none) { CHECK_EQ_I(_rmtmp(), 0); }
TC_CASE(counts_open_tmpfiles)
{
    FILE *a = tmpfile(), *b = tmpfile();
    if (!a || !b) { if (a) fclose(a); if (b) fclose(b); TC_SKIP("tmpfile failed"); }
    CHECK_EQ_I(_rmtmp(), 2);
}
TC_CASE(streams_closed_after)
{
    FILE *a = tmpfile(); int fd, r;
    if (!a) TC_SKIP("tmpfile failed");
    fd = _fileno(a);
    CHECK(fd >= 0);
    CHECK_EQ_I(_rmtmp(), 1);
    errno = 0; r = _eof(fd);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
}
TC_CASE(second_call_zero)
{
    FILE *a = tmpfile();
    if (!a) TC_SKIP("tmpfile failed");
    CHECK_EQ_I(_rmtmp(), 1);
    CHECK_EQ_I(_rmtmp(), 0);
}
TC_CASE(fclosed_tmpfile_not_counted)
{
    FILE *a = tmpfile();
    if (!a) TC_SKIP("tmpfile failed");
    CHECK_EQ_I(fclose(a), 0);
    CHECK_EQ_I(_rmtmp(), 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(zero_when_none); TC_RUN(counts_open_tmpfiles); TC_RUN(streams_closed_after);
    TC_RUN(second_call_zero); TC_RUN(fclosed_tmpfile_not_counted);
    return tc_end();
}
