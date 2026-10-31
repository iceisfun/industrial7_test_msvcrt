/* TC
 * id:    scn_neg_fscanf_bad_fd_probe
 * area:  scanf
 * kind:  negative
 * funcs: fscanf fopen fclose _fileno _close
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _read, fscanf (invalid file descriptor -> errno EBADF)
 * note:  PROBE: the stream's descriptor is closed behind the FILE's back (FILE is leaked on purpose, then fclose'd
 *        ignoring its result). The stream has an empty buffer, so fscanf must refill and fail: EOF with the target
 *        untouched, and errno EBADF. The handler count is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <io.h>

#define F "tc_scn_neg_badfd_probe.tmp"
static FILE *mk(void)
{
    FILE *f = fopen(F, "w");
    if (f) { fputs("42\n", f); fclose(f); f = fopen(F, "r"); }
    tc_iph_install();
    return f;
}
TC_CASE(result_eof)
{
    FILE *f = mk(); int a = 5, r; CHECK(f != NULL); if (!f) { remove(F); return; }
    _close(_fileno(f));
    r = fscanf(f, "%d", &a);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 5);
    (void)fclose(f); remove(F);
}
TC_CASE(errno_ebadf)
{
    FILE *f = mk(); int a = 5; CHECK(f != NULL); if (!f) { remove(F); return; }
    _close(_fileno(f));
    errno = 0; (void)fscanf(f, "%d", &a);
    CHECK_EQ_I(errno, EBADF);
    (void)fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(result_eof); TC_RUN(errno_ebadf); return tc_end(); }
