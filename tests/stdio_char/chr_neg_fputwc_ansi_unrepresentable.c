/* TC
 * id:    chr_neg_fputwc_ansi_unrepresentable
 * area:  stdio_char
 * funcs: fputwc
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.29.3.3 (encoding error: EILSEQ); MS docs fputwc
 * note:  PROBE: U+263A cannot be converted to a byte in the C locale: expect WEOF, EILSEQ and nothing written.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
#define FN "tc_chr_neg_fputwc_ansi_unrepresentable_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(unrepresentable)
{
    FILE *f; volatile wchar_t wc = 0x263A; wint_t r; int b;
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    errno = 0; r = fputwc(wc, f);
    CHECK(r == WEOF);
    CHECK_EQ_I(errno, EILSEQ);
    fclose(f);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    b = fgetc(f); CHECK_EQ_I(b, EOF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(unrepresentable); return tc_end(); }
