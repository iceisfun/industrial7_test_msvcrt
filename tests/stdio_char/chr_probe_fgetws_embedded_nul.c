/* TC
 * id:    chr_probe_fgetws_embedded_nul
 * area:  stdio_char
 * funcs: fgetws
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.29.3.2 (silent on embedded nul)
 * note:  PROBE: fgetws on a UTF-16LE file containing U+0000 mid-line stores the raw wchars (no early stop) so that
 *        wcslen==2 while buf[3]=='c' and buf[4]=='\n'. Believed true of MS fgetws (it copies characters until n-1 or newline).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static void wr(const char *n, const void *d, size_t len)
{ FILE *f = fopen(n, "wb"); if (f) { if (len) fwrite(d, 1, len, f); fclose(f); } }
static size_t rd(const char *n, unsigned char *b, size_t max)
{ FILE *f = fopen(n, "rb"); size_t r = 0; if (f) { r = fread(b, 1, max, f); fclose(f); } return r; }
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_chr_probe_fgetws_embedded_nul_a_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(embedded_nul)
{
    static const unsigned char d[] = {0xFF,0xFE, 'a',0, 'b',0, 0,0, 'c',0, '\n',0};
    wchar_t b[16]; FILE *f; int i; wr(FN, d, sizeof d);
    for (i = 0; i < 16; i++) b[i] = 0x4141;
    f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(b, 16, f) == b);
    CHECK_EQ_U(wcslen(b), 2);
    CHECK_EQ_U(b[2], 0); CHECK_EQ_U(b[3], L'c'); CHECK_EQ_U(b[4], L'\n'); CHECK_EQ_U(b[5], 0);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(embedded_nul); return tc_end(); }
