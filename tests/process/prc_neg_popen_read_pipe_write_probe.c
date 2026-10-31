/* TC
 * id:    prc_neg_popen_read_pipe_write_probe
 * area:  process
 * funcs: _popen _pclose fwrite fgetc ferror
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _popen (modes "r" and "w"); C11 7.21.8.2 (stream must permit the operation)
 * note:  PROBE: operating against the stream direction fails without hanging. fwrite on a "r" pipe returns 0 with ferror set or
 *        errno EBADF; fgetc on a "w" pipe returns EOF; _pclose then returns the child's exit status 0.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(write_to_read_pipe)
{
    FILE *f; size_t n; int e; int r;
    tc_iph_install();
    f = _popen("cmd /c exit 0", "r");
    CHECK(f != NULL); if (!f) return;
    errno = 0;
    n = fwrite("abc", 1, 3, f); e = errno;
    CHECK_EQ_U(n, 0);
    CHECK(ferror(f) != 0 || e == EBADF);
    r = _pclose(f);
    CHECK_EQ_I(r, 0);
}
TC_CASE(read_from_write_pipe)
{
    FILE *f; int c; int r;
    tc_iph_install();
    f = _popen("cmd /c exit 0", "w");
    CHECK(f != NULL); if (!f) return;
    c = fgetc(f);
    CHECK_EQ_I(c, EOF);
    r = _pclose(f);
    CHECK_EQ_I(r, 0);
}
int main(void) { tc_begin(); TC_RUN(write_to_read_pipe); TC_RUN(read_from_write_pipe); return tc_end(); }
