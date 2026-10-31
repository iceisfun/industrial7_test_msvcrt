/* TC
 * id:    chr_fputws_ccs_utf16
 * area:  stdio_char
 * funcs: fputws
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (ccs=UTF-16LE writes BOM, UTF-8 encoding), fputws
 * note:  UTF-8 stream: BOM presence is not asserted (only that the encoded bytes C3 A9 come last).
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
#define FN "tc_chr_fputws_ccs_utf16_" TC_S(TC_CRT_VER) ".tmp"
static const wchar_t *volatile vs;

TC_CASE(utf16_hi)
{
    static const unsigned char want[] = {0xFF,0xFE,'h',0,'i',0};
    unsigned char b[16]; size_t n; FILE *f = fopen(FN, "w,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    vs = L"hi"; CHECK(fputws(vs, f) >= 0);
    fclose(f); n = rd(FN, b, 16); CHECK_EQ_U(n, 6); CHECK_MEM(b, want, 6); remove(FN);
}
TC_CASE(utf16_newline)
{
    static const unsigned char want[] = {0xFF,0xFE,'a',0,0x0D,0,0x0A,0,'b',0};
    unsigned char b[16]; size_t n; FILE *f = fopen(FN, "w,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    vs = L"a\nb"; CHECK(fputws(vs, f) >= 0);
    fclose(f); n = rd(FN, b, 16); CHECK_EQ_U(n, 10); CHECK_MEM(b, want, 10); remove(FN);
}
TC_CASE(utf16_nonascii)
{
    static const unsigned char want[] = {0xFF,0xFE,0x3A,0x26,0xE9,0x00};
    unsigned char b[16]; size_t n; FILE *f = fopen(FN, "w,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    vs = L"\x263A\x00e9"; CHECK(fputws(vs, f) >= 0);
    fclose(f); n = rd(FN, b, 16); CHECK_EQ_U(n, 6); CHECK_MEM(b, want, 6); remove(FN);
}
TC_CASE(utf8_e_acute)
{
    unsigned char b[16]; size_t n; FILE *f = fopen(FN, "w,ccs=UTF-8"); CHECK(f != NULL); if (!f) return;
    vs = L"\x00e9"; CHECK(fputws(vs, f) >= 0);
    fclose(f); n = rd(FN, b, 16);
    CHECK(n == 2 || n == 5); if (n >= 2) { CHECK_EQ_U(b[n-2], 0xC3); CHECK_EQ_U(b[n-1], 0xA9); }
    remove(FN);
}
int main(void) { tc_begin(); TC_RUN(utf16_hi); TC_RUN(utf16_newline); TC_RUN(utf16_nonascii); TC_RUN(utf8_e_acute); return tc_end(); }
