/* TC
 * id:    fio_rewind_resets
 * area:  stdio_file
 * funcs: rewind fopen fgetc ftell fwrite fclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.9.5; MS docs rewind
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
static const char *fn = "tc_fio_rewind_resets_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(ftell_and_first_byte)
{
    FILE *fp;
    mk(fn, "xyz123", 6);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    fgetc(fp); fgetc(fp); fgetc(fp);
    CHECK_EQ_I(ftell(fp), 3);
    rewind(fp);
    CHECK_EQ_I(ftell(fp), 0);
    CHECK_EQ_I(fgetc(fp), 'x');
    fclose(fp); remove(fn);
}
TC_CASE(after_write_flushes)
{
    FILE *fp; char buf[8] = {0}; size_t n;
    fp = fopen(fn, "w+b"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_U(fwrite("hello", 1, 5, fp), 5);
    rewind(fp);
    CHECK_EQ_I(ftell(fp), 0);
    n = fread(buf, 1, sizeof buf - 1, fp);
    CHECK_EQ_U(n, 5); CHECK_STR(buf, "hello");
    fclose(fp); remove(fn);
}
TC_CASE(clears_eof)
{
    FILE *fp;
    mk(fn, "ab", 2);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    fgetc(fp); fgetc(fp); CHECK_EQ_I(fgetc(fp), EOF);
    CHECK(feof(fp) != 0);
    rewind(fp);
    CHECK_EQ_I(feof(fp), 0);
    CHECK_EQ_I(fgetc(fp), 'a');
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(ftell_and_first_byte); TC_RUN(after_write_flushes); TC_RUN(clears_eof); return tc_end(); }
