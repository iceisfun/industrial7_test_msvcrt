/* TC
 * id:    fio_neg_close_after_fclose
 * area:  stdio_file
 * funcs: _fdopen fclose _close
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fclose (closes the underlying file descriptor), _close
 * note:  Invalid parameter handler may also fire on _close of a closed fd; installed, count not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const char * volatile fn = "tc_fio_neg_close_after_fclose_a.tmp";

TC_CASE(close_after_fclose)
{
    int fd = _open(fn, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    FILE *f; int r;
    CHECK(fd >= 0); if (fd < 0) return;
    f = _fdopen(fd, "wb");
    CHECK(f != NULL); if (!f) { _close(fd); return; }
    CHECK_EQ_I(fclose(f), 0);
    errno = 0;
    r = _close(fd);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fn);
    CHECK(tc_iph_install());
    TC_RUN(close_after_fclose);
    DeleteFileA(fn);
    return tc_end();
}
