/* TC
 * id:    chr_nolock_wide_roundtrip
 * area:  stdio_char
 * funcs: _fputwc_nolock _fgetwc_nolock _ungetwc_nolock _lock_file _unlock_file
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fgetwc_nolock, _fputwc_nolock, _ungetwc_nolock
 * note:  _putwc_nolock/_getwc_nolock (macros in the MS headers) are in chr_probe_nolock_wide_aliases.
 *        ANSI-stream _ungetwc_nolock moved to chr_probe_ungetwc_ansi_text (unspecified byte storage).
 *        msvcrt.dll excluded: its import library lacks _fgetwc_nolock.
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
#define FN "tc_chr_nolock_wide_roundtrip_" TC_S(TC_CRT_VER) ".tmp"
static volatile wint_t vc = L'u';

TC_CASE(ascii_put)
{
    unsigned char b[8]; size_t n; FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    _lock_file(f);
    CHECK_EQ_U((_fputwc_nolock)(L'a', f), L'a'); CHECK_EQ_U((_fputwc_nolock)(L'b', f), L'b');
    _unlock_file(f);
    fclose(f); n = rd(FN, b, 8); CHECK_EQ_U(n, 2); CHECK_MEM(b, "ab", 2); remove(FN);
}
TC_CASE(ascii_get)
{
    FILE *f; wr(FN, "xy", 2); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    _lock_file(f);
    CHECK_EQ_U((_fgetwc_nolock)(f), L'x'); CHECK_EQ_U((_fgetwc_nolock)(f), L'y'); CHECK_EQ_U((_fgetwc_nolock)(f), WEOF);
    _unlock_file(f);
    fclose(f); remove(FN);
}
TC_CASE(utf16_nonascii)
{
    static const unsigned char want[] = {0xFF,0xFE,0x3A,0x26,'A',0,0xE9,0};
    unsigned char b[16]; size_t n; FILE *f = fopen(FN, "w,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    _lock_file(f);
    CHECK_EQ_U((_fputwc_nolock)(0x263A, f), 0x263A); CHECK_EQ_U((_fputwc_nolock)(L'A', f), L'A'); CHECK_EQ_U((_fputwc_nolock)(0xE9, f), 0xE9);
    _unlock_file(f);
    fclose(f); n = rd(FN, b, 16); CHECK_EQ_U(n, 8); CHECK_MEM(b, want, 8);
    f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    _lock_file(f);
    CHECK_EQ_U((_fgetwc_nolock)(f), 0x263A); CHECK_EQ_U((_fgetwc_nolock)(f), L'A');
    vc = 0x2603; CHECK_EQ_U((_ungetwc_nolock)(vc, f), 0x2603); CHECK_EQ_U((_fgetwc_nolock)(f), 0x2603);
    CHECK_EQ_U((_fgetwc_nolock)(f), 0xE9); CHECK_EQ_U((_fgetwc_nolock)(f), WEOF);
    _unlock_file(f);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(ascii_put); TC_RUN(ascii_get); TC_RUN(utf16_nonascii); return tc_end(); }
