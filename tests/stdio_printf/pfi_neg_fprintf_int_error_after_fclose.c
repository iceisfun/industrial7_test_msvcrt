/* TC
 * id:    pfi_neg_fprintf_int_error_after_fclose
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _close, fflush, _write (EBADF); C11 7.21.5.2 fflush returns EOF on write error
 * note:  The underlying fd is closed behind the FILE's back; only the fflush result and errno are asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

TC_CASE(closed_fd_flush)
{
    const char *name = "tc_pfi_neg_fprintf_int_error_after_fclose.tmp";
    int fd, fl; FILE *f;
    fd = _open(name, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    f = _fdopen(fd, "wb");
    CHECK(f != NULL);
    if (!f) { _close(fd); remove(name); return; }
    CHECK_EQ_I(_close(fd), 0);
    (void)fprintf(f, "%d", 4242);
    errno = 0;
    fl = fflush(f);
    CHECK_EQ_I(fl, EOF);
    CHECK_EQ_I(errno, EBADF);
    (void)fclose(f);
    remove(name);
}
TC_CASE(control_stream)
{
    const char *name = "tc_pfi_neg_fprintf_int_error_after_fclose2.tmp";
    FILE *f = fopen(name, "wb"); int n;
    CHECK(f != NULL); if (!f) return;
    n = fprintf(f, "%d", 4242); CHECK_EQ_I(n, 4);
    CHECK_EQ_I(fflush(f), 0);
    fclose(f); remove(name);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(closed_fd_flush); TC_RUN(control_stream);
    return tc_end();
}
