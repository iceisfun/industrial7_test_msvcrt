/* TC
 * id:    fio_ferror_clearerr
 * area:  stdio_file
 * funcs: clearerr feof fopen fclose fgetc ftell fwrite
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.10.1; MS docs clearerr
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
static const char *fn = "tc_fio_ferror_clearerr_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(clear_eof_keeps_pos)
{
    FILE *fp;
    mk(fn, "abc", 3);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    fgetc(fp); fgetc(fp); fgetc(fp); CHECK_EQ_I(fgetc(fp), EOF);
    CHECK(feof(fp) != 0);
    clearerr(fp);
    CHECK_EQ_I(feof(fp), 0);
    CHECK_EQ_I(ftell(fp), 3);
    CHECK_EQ_I(fgetc(fp), EOF);
    CHECK(feof(fp) != 0);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(clear_eof_keeps_pos); return tc_end(); }
