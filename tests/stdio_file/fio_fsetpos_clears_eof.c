/* TC
 * id:    fio_fsetpos_clears_eof
 * area:  stdio_file
 * funcs: fsetpos fgetpos feof fread fopen fclose fwrite
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.9.3 (fsetpos clears end-of-file indicator)
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
static const char *fn = "tc_fio_fsetpos_clears_eof_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(clears)
{
    FILE *fp; fpos_t p0; char buf[16]; size_t n;
    mk(fn, "hello", 5);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(fgetpos(fp, &p0), 0);
    n = fread(buf, 1, sizeof buf, fp); CHECK_EQ_U(n, 5);
    CHECK(feof(fp) != 0);
    CHECK_EQ_I(fsetpos(fp, &p0), 0);
    CHECK_EQ_I(feof(fp), 0);
    n = fread(buf, 1, 2, fp); CHECK_EQ_U(n, 2); CHECK_MEM(buf, "he", 2);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(clears); return tc_end(); }
