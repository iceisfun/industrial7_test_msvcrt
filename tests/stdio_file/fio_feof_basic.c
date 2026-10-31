/* TC
 * id:    fio_feof_basic
 * area:  stdio_file
 * funcs: feof fgetc fopen fclose fwrite
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.10.2; MS docs feof
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
static const char *fn = "tc_fio_feof_basic_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(timing)
{
    FILE *fp;
    mk(fn, "ab", 2);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(feof(fp), 0);
    CHECK_EQ_I(fgetc(fp), 'a');
    CHECK_EQ_I(fgetc(fp), 'b');
    CHECK_EQ_I(feof(fp), 0);
    CHECK_EQ_I(fgetc(fp), EOF);
    CHECK(feof(fp) != 0);
    fclose(fp); remove(fn);
}
TC_CASE(empty_file)
{
    FILE *fp;
    mk(fn, "", 0);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(feof(fp), 0);
    CHECK_EQ_I(fgetc(fp), EOF);
    CHECK(feof(fp) != 0);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(timing); TC_RUN(empty_file); return tc_end(); }
