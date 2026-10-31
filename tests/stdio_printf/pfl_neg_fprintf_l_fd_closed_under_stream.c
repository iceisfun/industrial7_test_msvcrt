/* TC
 * id:    pfl_neg_fprintf_l_fd_closed_under_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vfprintf_l _create_locale _free_locale fopen fclose fflush _close _fileno
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.5.2 (fflush: EOF on write error); MS docs _close, fflush
 * note:  PROBE: the stream's file descriptor is closed behind its back with _close(_fileno(f)). The buffered _fprintf_l write is
 *        not asserted (it may succeed into the buffer); the later flush must fail: fflush returns EOF with errno == EBADF.
 *        The invalid-parameter handler MAY run (the CRT's descriptor validation) and is therefore counted but not asserted.
 *        fclose on the stream afterwards must merely return without crashing. Temp file tc_pfl_neg_fprintf_l_fd_closed.tmp.
 */
#include "tc.h"
#include "pfl_shim.h"
#include <errno.h>
#include <io.h>

static const char *fname = "tc_pfl_neg_fprintf_l_fd_closed.tmp";

TC_CASE(flush_fails_ebadf)
{
    _locale_t l = _create_locale(LC_ALL, "C"); FILE *f; int r;
    CHECK(l != NULL); if (!l) return;
    f = fopen(fname, "w"); CHECK(f != NULL);
    if (f) {
        CHECK_EQ_I(_close(_fileno(f)), 0);
        pl_fprintf_l(f, "x", l);          /* buffered: result not asserted */
        errno = 0;
        r = fflush(f);
        CHECK_EQ_I(r, EOF);
        CHECK_EQ_I(errno, EBADF);
        r = fclose(f);                    /* must not crash; return value not asserted */
        (void)r;
    }
    _free_locale(l);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(flush_fails_ebadf);
    DeleteFileA(fname);
    return tc_end();
}
