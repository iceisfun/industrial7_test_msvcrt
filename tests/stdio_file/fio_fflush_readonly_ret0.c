/* TC
 * id:    fio_fflush_readonly_ret0
 * area:  stdio_file
 * funcs: fflush fopen fclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs fflush (returns 0 if the stream has no buffer or is open for reading only)
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static const char *fn = "tc_fio_fflush_readonly_ret0_a.tmp";

TC_CASE(readonly)
{
    FILE *fp = fopen(fn, "wb");
    volatile int rc;
    CHECK(fp != NULL); if (!fp) return;
    fwrite("hello", 1, 5, fp); fclose(fp);
    fp = fopen(fn, "rb");
    CHECK(fp != NULL); if (!fp) return;
    rc = fflush(fp);
    CHECK_EQ_I(rc, 0);
    CHECK_EQ_I(feof(fp), 0);
    CHECK_EQ_I(ferror(fp), 0);
    fclose(fp); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(readonly); return tc_end(); }
