/* TC
 * id:    fio_neg_probe_fclose_fd_closed
 * area:  stdio_file
 * funcs: fopen fclose _fileno
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fclose, _close
 * note:  PROBE: fclose on a stream whose descriptor was already closed with _close. Expected EOF + EBADF; whether the invalid-parameter handler fires is recorded only (0 or 1).\n */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static const char *fn = "tc_fio_neg_probe_fclose_fd_closed_a.tmp";

TC_CASE(fd_closed)
{
    FILE *fp = fopen(fn, "wb");
    volatile int rc, c;
    CHECK(fp != NULL); if (!fp) return;
    tc_iph_install();
    c = _close(_fileno(fp));
    CHECK_EQ_I(c, 0);
    tc_iph_count = 0; errno = 0;
    rc = fclose(fp);
    CHECK_EQ_I(rc, EOF);
    CHECK_EQ_I(errno, EBADF);
    CHECK(tc_iph_count == 0 || tc_iph_count == 1);
    DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(fd_closed); return tc_end(); }
