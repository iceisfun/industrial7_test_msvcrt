/* TC
 * id:    chr_neg_fgetwc_wide_on_ansi_eof_weof
 * area:  stdio_char
 * funcs: fgetwc fgetws feof ferror clearerr fopen fclose
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.3.1, 7.29.3.2 (fgetwc returns WEOF at end-of-file and sets the EOF indicator; fgetws leaves the array unchanged); MS docs fgetwc
 * note:  End of input on an empty file: WEOF, feof set, ferror clear; fgetws returns NULL and does not touch the buffer.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_chr_neg_fgetwc_wide_on_ansi_eof_weof_" TCS(TC_CRT_VER) ".tmp"

static FILE *open_empty(void)
{
    FILE *f = fopen(FN, "wb");
    if (f) fclose(f);
    return fopen(FN, "r");
}
TC_CASE(fgetwc_eof)
{
    FILE *f = open_empty(); wint_t c;
    CHECK(f != NULL); if (!f) { remove(FN); return; }
    c = fgetwc(f);
    CHECK_EQ_U(c, WEOF);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(ferror(f), 0);
    clearerr(f);
    CHECK_EQ_I(feof(f), 0);
    fclose(f); remove(FN);
}
TC_CASE(fgetws_eof_buffer_untouched)
{
    FILE *f = open_empty(); wchar_t buf[8]; wchar_t *p; int i;
    CHECK(f != NULL); if (!f) { remove(FN); return; }
    for (i = 0; i < 8; i++) buf[i] = (wchar_t)0x5a5a;
    p = fgetws(buf, 8, f);
    CHECK(p == NULL);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(ferror(f), 0);
    for (i = 0; i < 8; i++) CHECK_EQ_U(buf[i], 0x5a5a);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(fgetwc_eof); TC_RUN(fgetws_eof_buffer_untouched); return tc_end(); }
