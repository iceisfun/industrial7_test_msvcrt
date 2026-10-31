/* TC
 * id:    chr_neg_fgetwc_invalid_utf8
 * area:  stdio_char
 * funcs: fgetwc fopen fclose
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fopen (ccs=UTF-8), fgetwc
 * note:  PROBE: decoder error handling on a ccs=UTF-8 stream. Bytes 41 FF 42: the first character 'A' is certain,
 *        the probe is that the invalid byte 0xFF (never valid in UTF-8) makes fgetwc return WEOF. Narrowed to the UCRT:
 *        the hand-written UTF-8 decoders of msvcr80-120 may map invalid bytes differently (unknown), so they are not claimed.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define FN "tc_chr_neg_fgetwc_invalid_utf8.tmp"
static void mk(const char *name, const char *data, size_t n)
{
    FILE *f = fopen(name, "wb");
    if (f) { if (n) fwrite(data, 1, n, f); fclose(f); }
}
TC_CASE(bad_byte)
{
    FILE *f; wint_t a, b;
    mk(FN, "\x41\xFF\x42", 3);
    f = fopen(FN, "r, ccs=UTF-8");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    a = fgetwc(f);
    CHECK_EQ_U(a, 0x41);
    b = fgetwc(f);
    CHECK_EQ_U(b, WEOF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(bad_byte); return tc_end(); }
