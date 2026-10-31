/* TC
 * id:    chr_neg_fgetwc_truncated_utf16
 * area:  stdio_char
 * funcs: fgetwc fopen fclose
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fopen (ccs=UNICODE / UTF-16LE), fgetwc
 * note:  PROBE: file FF FE 41 00 42 is a UTF-16LE BOM, 'A', and one stray byte. 'A' is certain; the probe is that the
 *        incomplete trailing code unit yields WEOF rather than a garbage character.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define FN "tc_chr_neg_fgetwc_truncated_utf16.tmp"
static void mk(const char *name, const char *data, size_t n)
{
    FILE *f = fopen(name, "wb");
    if (f) { if (n) fwrite(data, 1, n, f); fclose(f); }
}
TC_CASE(odd_tail)
{
    FILE *f; wint_t a, b;
    mk(FN, "\xFF\xFE\x41\x00\x42", 5);
    f = fopen(FN, "r, ccs=UNICODE");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    a = fgetwc(f);
    CHECK_EQ_U(a, 0x41);
    b = fgetwc(f);
    CHECK_EQ_U(b, WEOF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(odd_tail); return tc_end(); }
