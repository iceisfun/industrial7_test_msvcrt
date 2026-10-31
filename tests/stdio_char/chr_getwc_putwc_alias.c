/* TC
 * id:    chr_getwc_putwc_alias
 * area:  stdio_char
 * funcs: getwc putwc fgetwc fputwc
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.3.6, 7.29.3.8; MS docs getwc, putwc
 * note:  parenthesised calls force the function rather than a macro.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static void wr(const char *n, const void *d, size_t len)
{ FILE *f = fopen(n, "wb"); if (f) { if (len) fwrite(d, 1, len, f); fclose(f); } }
static size_t rd(const char *n, unsigned char *b, size_t max)
{ FILE *f = fopen(n, "rb"); size_t r = 0; if (f) { r = fread(b, 1, max, f); fclose(f); } return r; }
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define FN "tc_chr_getwc_putwc_alias_" TC_S(TC_CRT_VER) ".tmp"
static volatile wchar_t vw = L'a';

TC_CASE(put_get)
{
    unsigned char b[8]; size_t n; FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U((putwc)(vw, f), L'a'); CHECK_EQ_U((fputwc)(L'b', f), L'b'); CHECK_EQ_U((putwc)(L'c', f), L'c');
    fclose(f); n = rd(FN, b, 8); CHECK_EQ_U(n, 3); CHECK_MEM(b, "abc", 3);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U((getwc)(f), L'a'); CHECK_EQ_U((fgetwc)(f), L'b'); CHECK_EQ_U((getwc)(f), L'c');
    CHECK_EQ_U((getwc)(f), WEOF); CHECK(feof(f) != 0);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(put_get); return tc_end(); }
