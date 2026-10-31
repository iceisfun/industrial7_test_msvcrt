/* TC
 * id:    chr_fgetws_ccs_utf16
 * area:  stdio_char
 * funcs: fgetws
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (ccs flag, BOM), fgetws; C11 7.29.3.2
 * note:  n=1 moved to chr_probe_fgets_n1; embedded NUL moved to chr_probe_fgetws_embedded_nul.
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
#define FN "tc_chr_fgetws_ccs_utf16_" TC_S(TC_CRT_VER) ".tmp"
static unsigned char raw[16000]; static size_t rn;
static void bom(void) { rn = 0; raw[rn++] = 0xFF; raw[rn++] = 0xFE; }
static void put(wchar_t c) { raw[rn++] = (unsigned char)(c & 0xFF); raw[rn++] = (unsigned char)(c >> 8); }
static void puts16(const char *s) { while (*s) put((wchar_t)*s++); }

TC_CASE(crlf_split)
{
    wchar_t b[32]; FILE *f; bom(); puts16("ab\r\ncd"); wr(FN, raw, rn);
    f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(b, 32, f) == b); CHECK_WSTR(b, L"ab\n");
    CHECK(fgetws(b, 32, f) == b); CHECK_WSTR(b, L"cd");
    CHECK(fgetws(b, 32, f) == NULL);
    fclose(f); remove(FN);
}
TC_CASE(long_line_chunks)
{
    wchar_t b[101]; FILE *f; size_t total = 0, chunks = 0, i; int ok = 1; wchar_t *r;
    bom(); for (i = 0; i < 5000; i++) put(L'x'); puts16("\r\n"); wr(FN, raw, rn);
    f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    while (chunks < 100 && (r = fgetws(b, 101, f)) != NULL) {
        size_t n = wcslen(b); chunks++; total += n;
        if (n > 100) ok = 0;
        for (i = 0; i < n; i++) if (b[i] != L'x' && !(b[i] == L'\n' && i == n - 1)) ok = 0;
    }
    CHECK(ok); CHECK_EQ_U(total, 5001); CHECK_EQ_U(chunks, 51);
    CHECK_WSTR(b, L"\n");
    fclose(f); remove(FN);
}
TC_CASE(n2)
{
    wchar_t b[8]; FILE *f; bom(); puts16("xyz"); wr(FN, raw, rn);
    f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(b, 2, f) == b); CHECK_WSTR(b, L"x");
    CHECK(fgetws(b, 8, f) == b); CHECK_WSTR(b, L"yz");
    fclose(f); remove(FN);
}
TC_CASE(bom_not_returned)
{
    FILE *f; wint_t c; bom(); put(0x263A); puts16("A"); wr(FN, raw, rn);
    f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 0x263A); c = fgetwc(f); CHECK_EQ_U(c, L'A');
    CHECK_EQ_U(fgetwc(f), WEOF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(crlf_split); TC_RUN(long_line_chunks); TC_RUN(n2); TC_RUN(bom_not_returned); return tc_end(); }
