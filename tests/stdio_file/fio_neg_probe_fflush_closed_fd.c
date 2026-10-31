/* TC
 * id:    fio_neg_probe_fflush_closed_fd
 * area:  stdio_file
 * funcs: fflush fopen fwrite _fileno
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fflush, _close
 * note:  PROBE: fflush when the descriptor was closed underneath the stream: expect EOF, errno EBADF, ferror set.\n */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static const char *fn = "tc_fio_neg_probe_fflush_closed_fd_a.tmp";

TC_CASE(dead_fd)
{
    FILE *fp = fopen(fn, "wb");
    volatile int rc, c;
    CHECK(fp != NULL); if (!fp) return;
    tc_iph_install();
    CHECK_EQ_U(fwrite("data", 1, 4, fp), 4);
    c = _close(_fileno(fp));
    CHECK_EQ_I(c, 0);
    errno = 0;
    rc = fflush(fp);
    CHECK_EQ_I(rc, EOF);
    CHECK_EQ_I(errno, EBADF);
    CHECK(ferror(fp) != 0);
    clearerr(fp);
    fclose(fp);
    DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(dead_fd); return tc_end(); }
