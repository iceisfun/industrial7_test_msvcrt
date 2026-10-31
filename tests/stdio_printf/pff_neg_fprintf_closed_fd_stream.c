/* TC
 * id:    pff_neg_fprintf_closed_fd_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fflush
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fflush / _close; C11 7.21.5.2
 * note:  PROBES a FILE whose descriptor was closed behind its back with _close: buffered fprintf may succeed but fflush returns EOF
 *        with errno EBADF; with _IONBF fprintf leaves errno EBADF (its own return value is not asserted: the failing write happens in the implicit flush); ferror set afterwards and clearerr resets it.
 *        The invalid-parameter handler may or may not run for the bad descriptor; its count is not asserted.
 *        No file is opened between the _close and the fclose, so the freed descriptor number is not reused.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <io.h>

#define TMP "tc_pff_neg_fprintf_closed_fd_stream.tmp"

TC_CASE(buffered_flush_fails)
{
    FILE *f = fopen(TMP, "w"); int fr;
    CHECK(f != NULL); if (!f) return;
    _close(_fileno(f));
    fprintf(f, "%.0f", 2.5);
    errno = 0; fr = fflush(f);
    CHECK_EQ_I(fr, EOF); CHECK_EQ_I(errno, EBADF);
    CHECK(ferror(f) != 0);
    clearerr(f);
    CHECK_EQ_I(ferror(f), 0);
    fclose(f);
}
TC_CASE(unbuffered_fprintf_fails)
{
    FILE *f = fopen(TMP, "w"); int r;
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(setvbuf(f, NULL, _IONBF, 0), 0);
    _close(_fileno(f));
    errno = 0; r = fprintf(f, "%.0f", 2.5); (void)r;  /* return value not asserted: the write error may surface only at the implicit flush */
    CHECK_EQ_I(errno, EBADF);
    CHECK(ferror(f) != 0);
    clearerr(f);
    CHECK_EQ_I(ferror(f), 0);
    fclose(f);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(buffered_flush_fails); TC_RUN(unbuffered_fprintf_fails);
    remove(TMP);
    return tc_end();
}
