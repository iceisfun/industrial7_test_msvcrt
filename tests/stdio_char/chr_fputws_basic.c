/* TC
 * id:    chr_fputws_basic
 * area:  stdio_char
 * funcs: fputws
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.3.4; MS docs fputws; MS docs "Unicode stream I/O in text and binary modes" (binary mode: wide chars written as-is, no translation)
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
#define FN "tc_chr_fputws_basic_" TC_S(TC_CRT_VER) ".tmp"
static const wchar_t *volatile vs;

TC_CASE(plain_no_newline)
{
    unsigned char b[16]; size_t n; FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    vs = L"abc"; CHECK(fputws(vs, f) >= 0);
    vs = L"de"; CHECK(fputws(vs, f) >= 0);
    fclose(f); n = rd(FN, b, 16); CHECK_EQ_U(n, 5); CHECK_MEM(b, "abcde", 5); remove(FN);
}
TC_CASE(empty)
{
    unsigned char b[4]; size_t n; FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    vs = L""; CHECK(fputws(vs, f) >= 0);
    fclose(f); n = rd(FN, b, 4); CHECK_EQ_U(n, 0); remove(FN);
}
TC_CASE(stops_at_nul)
{
    static const wchar_t s[] = L"xy\0zw";
    unsigned char b[8]; size_t n; FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    vs = s; CHECK(fputws(vs, f) >= 0);
    fclose(f); n = rd(FN, b, 8); CHECK_EQ_U(n, 2); CHECK_MEM(b, "xy", 2); remove(FN);
}
TC_CASE(text_newline)
{
    unsigned char b[8]; size_t n; FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    vs = L"a\nb"; CHECK(fputws(vs, f) >= 0);
    fclose(f); n = rd(FN, b, 8); CHECK_EQ_U(n, 4); CHECK_MEM(b, "a\r\nb", 4); remove(FN);
}
TC_CASE(binary_newline)
{
    unsigned char b[8]; size_t n; FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    vs = L"a\nb"; CHECK(fputws(vs, f) >= 0);
    fclose(f); n = rd(FN, b, 8); CHECK_EQ_U(n, 6); CHECK_MEM(b, "a\0\n\0b\0", 6); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(plain_no_newline); TC_RUN(empty); TC_RUN(stops_at_nul); TC_RUN(text_newline); TC_RUN(binary_newline); return tc_end(); }
