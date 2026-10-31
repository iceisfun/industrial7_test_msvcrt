/* TC
 * id:    fio_fgetpos_text_crlf
 * area:  stdio_file
 * funcs: fgetpos fsetpos fopen fgets fclose fwrite
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   C11 7.21.9.1; MS docs fgetpos (fpos_t is __int64)
 * note:  round trip is standard; the numeric value 3 relies on MS fpos_t being a byte offset
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
static const char *fn = "tc_fio_fgetpos_text_crlf_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(roundtrip)
{
    FILE *fp; fpos_t pos; char a[32], b[32]; char *r;
    mk(fn, "a\r\nbcd\r\nef\r\n", 12);
    fp = fopen(fn, "r"); CHECK(fp != NULL); if (!fp) return;
    r = fgets(a, sizeof a, fp); CHECK(r != NULL); CHECK_STR(a, "a\n");
    CHECK_EQ_I(fgetpos(fp, &pos), 0);
    r = fgets(a, sizeof a, fp); CHECK(r != NULL); CHECK_STR(a, "bcd\n");
    CHECK_EQ_I(fsetpos(fp, &pos), 0);
    r = fgets(b, sizeof b, fp); CHECK(r != NULL); CHECK_STR(b, "bcd\n");
    r = fgets(b, sizeof b, fp); CHECK(r != NULL); CHECK_STR(b, "ef\n");
    fclose(fp); remove(fn);
}
TC_CASE(byte_offset)
{
    FILE *fp; fpos_t pos = -7; char a[32]; char *r;
    mk(fn, "a\r\nbcd\r\n", 8);
    fp = fopen(fn, "r"); CHECK(fp != NULL); if (!fp) return;
    r = fgets(a, sizeof a, fp); CHECK(r != NULL);
    CHECK_EQ_I(fgetpos(fp, &pos), 0);
    CHECK_EQ_I(pos, 3);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(roundtrip); TC_RUN(byte_offset); return tc_end(); }
