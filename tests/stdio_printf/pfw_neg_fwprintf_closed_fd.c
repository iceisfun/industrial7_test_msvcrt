/* TC
 * id:    pfw_neg_fwprintf_closed_fd
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf _close _fileno
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _write (EBADF for an invalid file descriptor), fflush (returns EOF on write error), ferror
 * note:  PROBE. The FILE's descriptor is closed behind its back. Probes that buffered fwprintf succeeds into the buffer and the flush then fails
 *        with EOF/EBADF and ferror set (that the wide write is buffered rather than rejected early is an implementation detail).
 *        The invalid-parameter-handler count is NOT asserted here (see pfw_neg_fwprintf_closed_fd_iph_probe).
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>
#include <wchar.h>

static const char tmp[] = "tc_pfw_neg_fwprintf_closed_fd.tmp";
static const wchar_t * volatile xfmt = L"x";

TC_CASE(flush_fails)
{
    FILE *fp = fopen(tmp, "w"); int r;
    CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(_close(_fileno(fp)), 0);
    (void)fwprintf(fp, xfmt);
    errno = 0; r = fflush(fp);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EBADF);
    CHECK(ferror(fp) != 0);
    (void)fclose(fp);   /* may return EOF; tolerated */
    remove(tmp);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(flush_fails); return tc_end(); }
