/* TC
 * id:    chr_neg_putw_failure_return_readonly
 * area:  stdio_char
 * funcs: _putw fopen fclose ferror fread
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _putw (returns the value written; EOF on error, use ferror to confirm)
 * note:  _putw on a read-only stream returns EOF and writes nothing; success returns the value written.
 */
#include "tc.h"
#include <stdio.h>
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_chr_neg_putw_failure_return_readonly_" TCS(TC_CRT_VER) ".tmp"

TC_CASE(readonly_fails)
{
    FILE *f; int r; char b[16] = {0}; size_t n;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    fwrite("abcd", 1, 4, f); fclose(f);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    r = _putw(0x12345678, f);
    CHECK_EQ_I(r, EOF);
    CHECK(ferror(f) != 0);
    fclose(f);
    f = fopen(FN, "rb"); CHECK(f != NULL);
    if (f) { n = fread(b, 1, sizeof b, f); CHECK_EQ_U(n, 4); CHECK_MEM(b, "abcd", 4); fclose(f); }
    remove(FN);
}
TC_CASE(success_contrast)
{
    FILE *f; int r;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    r = _putw(5, f);
    CHECK_EQ_I(r, 5);
    CHECK_EQ_I(ferror(f), 0);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(readonly_fails); TC_RUN(success_contrast); return tc_end(); }
