/* TC
 * id:    fio_ferror_clearerr_probe_wr_rdonly
 * area:  stdio_file
 * funcs: ferror clearerr fwrite fopen fclose
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.10.1; MS docs clearerr
 * note:  PROBE: fwrite of 1 byte to a stream opened "rb" returns 0 and sets the error indicator (ISO C does
 *        not define writing to an input-only stream; MS CRT is believed to set _IOERR with errno EBADF).
 *        clearerr clearing it is standard once the precondition holds.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void mk(const char *n, const char *d, size_t len)
{
    FILE *f = fopen(n, "wb");
    if (f) { fwrite(d, 1, len, f); fclose(f); }
}
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_fio_ferror_clearerr_probe_wr_rdonly_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(clear_error)
{
    FILE *fp;
    mk(fn, "abc", 3);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_U(fwrite("Z", 1, 1, fp), 0);
    CHECK(ferror(fp) != 0);
    clearerr(fp);
    CHECK_EQ_I(ferror(fp), 0);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(clear_error); return tc_end(); }
