/* TC
 * id:    fio_neg_fdopen_null_mode
 * area:  stdio_file
 * funcs: _fdopen
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fdopen (parameter validation not explicit)
 * note:  PROBE: _fdopen(fd,NULL) -> NULL, EINVAL, handler once. NULL mode: older msvcr80/90 may dereference it without validation, so only ucrtbase is claimed.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const char * volatile fn = "tc_fio_neg_fdopen_null_mode_a.tmp";
static const char * volatile nul = NULL;

TC_CASE(null_mode)
{
    int fd = _open(fn, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    FILE *f;
    CHECK(fd >= 0); if (fd < 0) return;
    tc_iph_count = 0; errno = 0;
    f = _fdopen(fd, nul);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (f) fclose(f); else _close(fd);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fn);
    CHECK(tc_iph_install());
    TC_RUN(null_mode);
    DeleteFileA(fn);
    return tc_end();
}
