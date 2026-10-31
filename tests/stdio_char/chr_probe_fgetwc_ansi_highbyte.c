/* TC
 * id:    chr_probe_fgetwc_ansi_highbyte
 * area:  stdio_char
 * funcs: fgetwc
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs fgetwc (ANSI text stream converts via mbtowc)
 * note:  PROBE: C-locale single-byte to wide mapping of bytes >= 0x80 on a text-mode ANSI stream.
 *        UCRT C locale expected identity (0xE9 -> 0x00E9); older msvcrt may differ.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void mk(const char *name, const char *data, size_t n)
{
    FILE *f = fopen(name, "wb");
    if (f) { if (n) fwrite(data, 1, n, f); fclose(f); }
}

#include <wchar.h>
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_chr_probe_fgetwc_ansi_highbyte_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(byte_e9)
{
    FILE *f; mk(FN, "\xE9", 1);
    f = fopen(FN, "r");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    CHECK_EQ_U(fgetwc(f), 0x00E9);
    fclose(f); remove(FN);
}
TC_CASE(byte_80)
{
    FILE *f; mk(FN, "\x80", 1);
    f = fopen(FN, "r");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    CHECK_EQ_U(fgetwc(f), 0x0080);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(byte_e9); TC_RUN(byte_80); return tc_end(); }
