/* TC
 * id:    fio_fgetpos_fsetpos_roundtrip
 * area:  stdio_file
 * funcs: fgetpos fsetpos fopen fgetc fclose fwrite
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.9.1, 7.21.9.3; MS docs fgetpos, fsetpos
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
static const char *fn = "tc_fio_fgetpos_fsetpos_roundtrip_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(at_offset3)
{
    FILE *fp; fpos_t pos; volatile int rc; int c;
    mk(fn, "0123456789", 10);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    fgetc(fp); fgetc(fp); fgetc(fp);
    rc = fgetpos(fp, &pos); CHECK_EQ_I(rc, 0);
    c = fgetc(fp); CHECK_EQ_I(c, '3');
    fgetc(fp); fgetc(fp);
    rc = fsetpos(fp, &pos); CHECK_EQ_I(rc, 0);
    c = fgetc(fp); CHECK_EQ_I(c, '3');
    fclose(fp); remove(fn);
}
TC_CASE(start_and_eof)
{
    FILE *fp; fpos_t p0, pe; volatile int rc; int c;
    mk(fn, "0123456789", 10);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    rc = fgetpos(fp, &p0); CHECK_EQ_I(rc, 0);
    while ((c = fgetc(fp)) != EOF) {}
    rc = fgetpos(fp, &pe); CHECK_EQ_I(rc, 0);
    rc = fsetpos(fp, &p0); CHECK_EQ_I(rc, 0);
    c = fgetc(fp); CHECK_EQ_I(c, '0');
    rc = fsetpos(fp, &pe); CHECK_EQ_I(rc, 0);
    c = fgetc(fp); CHECK_EQ_I(c, EOF);
    fclose(fp); remove(fn);
}
TC_CASE(after_write_rplus)
{
    FILE *fp; fpos_t pos; volatile int rc; int c;
    mk(fn, "0123456789", 10);
    fp = fopen(fn, "r+b"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_U(fwrite("AB", 1, 2, fp), 2);
    rc = fgetpos(fp, &pos); CHECK_EQ_I(rc, 0);
    CHECK_EQ_U(fwrite("CD", 1, 2, fp), 2);
    rc = fsetpos(fp, &pos); CHECK_EQ_I(rc, 0);
    c = fgetc(fp); CHECK_EQ_I(c, 'C');
    c = fgetc(fp); CHECK_EQ_I(c, 'D');
    c = fgetc(fp); CHECK_EQ_I(c, '4');
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(at_offset3); TC_RUN(start_and_eof); TC_RUN(after_write_rplus); return tc_end(); }
