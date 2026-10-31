/* TC
 * id:    chr_neg_fputwc_utf8_lone_surrogate_probe
 * area:  stdio_char
 * funcs: fputwc fopen fclose fread
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fputwc, fopen ccs=
 * note:  PROBE: fputwc(0xD800) on a "w,ccs=UTF-8" stream. A lone surrogate is not a valid scalar value; the CRT either
 *        rejects it (WEOF), encodes it (3 bytes ED A0 80) or holds it pending as half a pair. Only "defined outcome, no crash,
 *        UTF-8 BOM written" is asserted; the bytes after the BOM are not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_chr_neg_fputwc_utf8_lone_surrogate_probe_" TCS(TC_CRT_VER) ".tmp"

static long slurp(unsigned char *b, long cap)
{
    FILE *f = fopen(FN, "rb"); long n;
    if (!f) return -1;
    n = (long)fread(b, 1, (size_t)cap, f);
    fclose(f);
    return n;
}
TC_CASE(lone_high_surrogate)
{
    FILE *f; wint_t r, r2; unsigned char b[32]; long n;
    f = fopen(FN, "w,ccs=UTF-8");
    CHECK(f != NULL);
    if (!f) return;
    errno = 0;
    r = fputwc((wchar_t)0xD800, f);
    CHECK(r == WEOF || r == 0xD800);
    if (r == WEOF) CHECK(errno != 0);
    r2 = fputwc(L'A', f);
    CHECK(r2 == L'A' || r2 == WEOF);   /* a pending lead surrogate may make the next unit fail */
    fclose(f);
    n = slurp(b, sizeof b);
    CHECK(n >= 3);
    if (n >= 3) {   /* only the documented BOM is asserted; the surrogate encoding is what is being probed */
        CHECK_EQ_U(b[0], 0xEF); CHECK_EQ_U(b[1], 0xBB); CHECK_EQ_U(b[2], 0xBF);
    }
    remove(FN);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(lone_high_surrogate); return tc_end(); }
