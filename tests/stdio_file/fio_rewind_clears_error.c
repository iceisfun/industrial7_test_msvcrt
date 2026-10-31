/* TC
 * id:    fio_rewind_clears_error
 * area:  stdio_file
 * funcs: rewind fwrite ferror fopen fclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.9.5 (rewind clears error indicator)
 * note:  PROBE: the precondition (fwrite to a read-only "rb" stream returns 0 and sets the error indicator) is
 *        not defined by ISO C; it is how the MS CRT is believed to behave (EBADF + _IOERR). The rewind
 *        clearing of the indicator is standard once the precondition holds.
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
static const char *fn = "tc_fio_rewind_clears_error_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(clears)
{
    FILE *fp; size_t n;
    mk(fn, "abc", 3);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    n = fwrite("Z", 1, 1, fp);
    CHECK_EQ_U(n, 0);
    CHECK(ferror(fp) != 0);
    rewind(fp);
    CHECK_EQ_I(ferror(fp), 0);
    CHECK_EQ_I(fgetc(fp), 'a');
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(clears); return tc_end(); }
