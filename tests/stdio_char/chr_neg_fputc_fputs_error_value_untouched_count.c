/* TC
 * id:    chr_neg_fputc_fputs_error_value_untouched_count
 * area:  stdio_char
 * funcs: fputc fputs ftell fseek fopen fclose fread ferror clearerr
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.21.7.3, 7.21.7.8; MS docs fputc, fputs (return EOF on error)
 * note:  Writes to a read-only ("r") stream fail with EOF and must not move the file position or modify the file.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_chr_neg_fputc_fputs_error_value_untouched_count_" TCS(TC_CRT_VER) ".tmp"

static void mk(void)
{
    FILE *f = fopen(FN, "wb");
    if (f) { fwrite("abcdef", 1, 6, f); fclose(f); }
}
static void verify_unchanged(void)
{
    FILE *f = fopen(FN, "rb"); char b[16] = {0}; size_t n;
    CHECK(f != NULL); if (!f) return;
    n = fread(b, 1, sizeof b, f);
    CHECK_EQ_U(n, 6);
    CHECK_MEM(b, "abcdef", 6);
    fclose(f);
}
TC_CASE(fputc_readonly)
{
    FILE *f; int r;
    mk(); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(fseek(f, 2, SEEK_SET), 0);
    CHECK_EQ_I(ftell(f), 2);
    r = (fputc)('x', f);
    CHECK_EQ_I(r, EOF);
    CHECK(ferror(f) != 0);
    clearerr(f);
    CHECK_EQ_I(ftell(f), 2);
    fclose(f);
    verify_unchanged(); remove(FN);
}
TC_CASE(fputs_readonly)
{
    FILE *f; int r;
    mk(); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(fseek(f, 2, SEEK_SET), 0);
    r = fputs("xyz", f);
    CHECK_EQ_I(r, EOF);
    CHECK(ferror(f) != 0);
    clearerr(f);
    CHECK_EQ_I(ftell(f), 2);
    fclose(f);
    verify_unchanged(); remove(FN);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fputc_readonly); TC_RUN(fputs_readonly); return tc_end(); }
