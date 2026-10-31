/* TC
 * id:    chr_ungetwc_basic
 * area:  stdio_char
 * funcs: ungetwc fgetwc
 * crt:   >=80
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.3.10; MS docs ungetwc
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
#define FN "tc_chr_ungetwc_basic_" TC_S(TC_CRT_VER) ".tmp"
static volatile wint_t vc;

static FILE *mk(void)
{
    static const unsigned char d[] = {0xFF,0xFE,'A',0,'B',0,0x3A,0x26};
    wr(FN, d, sizeof d); return fopen(FN, "r,ccs=UTF-16LE");
}
TC_CASE(same_char)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fgetwc(f), L'A'); vc = L'A';
    CHECK_EQ_U(ungetwc(vc, f), L'A');
    CHECK_EQ_U(fgetwc(f), L'A'); CHECK_EQ_U(fgetwc(f), L'B');
    fclose(f); remove(FN);
}
TC_CASE(nonascii)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fgetwc(f), L'A'); vc = 0x263A;
    CHECK_EQ_U(ungetwc(vc, f), 0x263A);
    CHECK_EQ_U(fgetwc(f), 0x263A);
    fclose(f); remove(FN);
}
TC_CASE(different_char)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fgetwc(f), L'A'); CHECK_EQ_U(fgetwc(f), L'B'); vc = L'Q';
    CHECK_EQ_U(ungetwc(vc, f), L'Q');
    CHECK_EQ_U(fgetwc(f), L'Q'); CHECK_EQ_U(fgetwc(f), 0x263A);
    fclose(f); remove(FN);
}
TC_CASE(then_eof)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fgetwc(f), L'A'); CHECK_EQ_U(fgetwc(f), L'B'); CHECK_EQ_U(fgetwc(f), 0x263A);
    CHECK_EQ_U(fgetwc(f), WEOF); CHECK(feof(f) != 0);
    vc = L'Z'; CHECK_EQ_U(ungetwc(vc, f), L'Z'); CHECK(!feof(f));
    CHECK_EQ_U(fgetwc(f), L'Z'); CHECK_EQ_U(fgetwc(f), WEOF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(same_char); TC_RUN(nonascii); TC_RUN(different_char); TC_RUN(then_eof); return tc_end(); }
