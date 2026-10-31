/* TC
 * id:    chr_fgetws_basic
 * area:  stdio_char
 * funcs: fgetws
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.3.2; MS docs fgets, fgetws
 * note:  n=1 case moved to chr_probe_fgets_n1.
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
#define FN "tc_chr_fgetws_basic_" TC_S(TC_CRT_VER) ".tmp"

TC_CASE(lines)
{
    wchar_t b[32]; FILE *f; wr(FN, "ab\nc\nlast", 9);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(b, 32, f) == b); CHECK_WSTR(b, L"ab\n");
    CHECK(fgetws(b, 32, f) == b); CHECK_WSTR(b, L"c\n");
    CHECK(fgetws(b, 32, f) == b); CHECK_WSTR(b, L"last");
    CHECK(fgetws(b, 32, f) == NULL);
    fclose(f); remove(FN);
}
TC_CASE(n2)
{
    wchar_t b[8]; FILE *f; wr(FN, "xyz", 3);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    b[2] = 0x5555;
    CHECK(fgetws(b, 2, f) == b); CHECK_WSTR(b, L"x"); CHECK_EQ_U(b[2], 0x5555);
    CHECK(fgetws(b, 8, f) == b); CHECK_WSTR(b, L"yz");
    fclose(f); remove(FN);
}
TC_CASE(eof_untouched)
{
    wchar_t b[8]; FILE *f; int i; wr(FN, "", 0);
    for (i = 0; i < 8; i++) b[i] = 0x4141;
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(b, 8, f) == NULL); CHECK(feof(f) != 0);
    for (i = 0; i < 8; i++) CHECK_EQ_U(b[i], 0x4141);
    fclose(f); remove(FN);
}
TC_CASE(binary_raw_wchars)
{
    static const unsigned char d[] = {'a',0,'\r',0,'\n',0};
    wchar_t b[16]; FILE *f; wr(FN, d, sizeof d);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(b, 16, f) == b); CHECK_WSTR(b, L"a\r\n");
    fclose(f); remove(FN);
}
TC_CASE(crlf_text)
{
    wchar_t b[16]; FILE *f; wr(FN, "a\r\nb\r\n", 6);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(b, 16, f) == b); CHECK_WSTR(b, L"a\n");
    CHECK(fgetws(b, 16, f) == b); CHECK_WSTR(b, L"b\n");
    CHECK(fgetws(b, 16, f) == NULL);
    fclose(f);
    remove(FN);
}
int main(void) { tc_begin(); TC_RUN(lines); TC_RUN(n2); TC_RUN(eof_untouched); TC_RUN(crlf_text); TC_RUN(binary_raw_wchars); return tc_end(); }
