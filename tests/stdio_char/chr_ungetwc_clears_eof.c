/* TC
 * id:    chr_ungetwc_clears_eof
 * area:  stdio_char
 * funcs: ungetwc fgetwc
 * crt:   >=80
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.3.10p4 (successful ungetwc clears the end-of-file indicator)
 * note:  uses a ccs=UTF-16LE stream (ccs needs msvcr80+). The ANSI-text-stream variant is in chr_probe_ungetwc_ansi_text:
 *        how MS ungetwc stores a wide char in a narrow text stream's byte buffer is not documented.
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
#define FN "tc_chr_ungetwc_clears_eof_" TC_S(TC_CRT_VER) ".tmp"
static volatile wint_t vc = L'x';

TC_CASE(clears)
{
    FILE *f; wr(FN, "\xff\xfe" "a\0", 4); f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fgetwc(f), L'a');
    CHECK_EQ_U(fgetwc(f), WEOF); CHECK(feof(f) != 0);
    CHECK_EQ_U(ungetwc(vc, f), L'x'); CHECK(feof(f) == 0);
    CHECK_EQ_U(fgetwc(f), L'x'); CHECK_EQ_U(fgetwc(f), WEOF); CHECK(feof(f) != 0);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(clears); return tc_end(); }
