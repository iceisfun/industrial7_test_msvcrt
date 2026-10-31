/* TC
 * id:    err_neg_probe_perror_readonly_stderr
 * area:  errno_param
 * kind:  negative
 * funcs: perror
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs perror, freopen
 * note:  PROBE: stderr reopened read-only; perror must not crash. Whether the invalid-parameter handler runs and what errno is
 *        afterwards are recorded as "TC INFO" lines only (not asserted). stderr is redirected to NUL afterwards.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

TC_CASE(readonly)
{
    char b[24]; const char *fn = "tc_err_neg_probe_perror_ro.tmp"; int fd; volatile const char *p = "pfx";
    FILE *r;
    fd = _open(fn, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    _write(fd, "x\n", 2); _close(fd);
    r = freopen(fn, "r", stderr);
    CHECK(r != NULL);
    tc_iph_count = 0;
    errno = ENOENT; perror((const char *)p);
    tc_write("TC INFO perror_ro iph="); tc_i64(tc_iph_count, b); tc_write(b);
    tc_write(" errno="); tc_i64(errno, b); tc_write(b); tc_write("\n");
    freopen("NUL", "w", stderr);
    remove(fn);
    CHECK(1);  /* reaching here without a crash/termination is the assertion */
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(readonly); errno = 0; return tc_end(); }
