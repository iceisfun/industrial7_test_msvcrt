/* TC
 * id:    gsf_clearerr_s_basic
 * area:  stdio_file
 * funcs: clearerr_s fopen fgetc feof ferror fclose
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs clearerr_s (returns 0; EINVAL for NULL stream)
 * note:  msvcr80 omitted (mingw lib lacks clearerr_s); msvcrt.dll omitted (export unsure).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static void mk(const char *n, const char *d, size_t len)
{
    FILE *f = fopen(n, "wb");
    if (f) { fwrite(d, 1, len, f); fclose(f); }
}
static const char *fn = "tc_gsf_clearerr_s_basic_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(eof_cleared)
{
    FILE *fp; mk(fn, "ab", 2);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    fgetc(fp); fgetc(fp); CHECK_EQ_I(fgetc(fp), EOF);
    CHECK(feof(fp) != 0);
    CHECK_EQ_I(clearerr_s(fp), 0);
    CHECK_EQ_I(feof(fp), 0);
    fclose(fp); remove(fn);
}
TC_CASE(error_cleared)
{
    FILE *fp = fopen(fn, "wb"); CHECK(fp != NULL); if (!fp) return;
    CHECK(tc_iph_install());
    errno = 0;
    (void)fgetc(fp);   /* read on write-only stream: expected to set the error flag; not itself asserted */
    if (!ferror(fp)) { fclose(fp); remove(fn); TC_SKIP("setup: no error flag"); }
    CHECK_EQ_I(clearerr_s(fp), 0);
    CHECK_EQ_I(ferror(fp), 0);
    errno = 0;
    fclose(fp); remove(fn);
}
TC_CASE(clean_stream_noop)
{
    FILE *fp; mk(fn, "ab", 2);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(clearerr_s(fp), 0);
    CHECK_EQ_I(feof(fp), 0); CHECK_EQ_I(ferror(fp), 0);
    fclose(fp); remove(fn);
}
TC_CASE(position_kept)
{
    FILE *fp; mk(fn, "ab", 2);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    fgetc(fp); fgetc(fp); fgetc(fp);
    CHECK(feof(fp) != 0);
    CHECK_EQ_I(clearerr_s(fp), 0);
    CHECK_EQ_I(ftell(fp), 2);
    CHECK_EQ_I(fgetc(fp), EOF);
    CHECK(feof(fp) != 0);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(eof_cleared); TC_RUN(error_cleared); TC_RUN(clean_stream_noop); TC_RUN(position_kept); return tc_end(); }
