/* TC
 * id:    chr_neg_fgetwc_ffff_data_ambiguity_probe
 * area:  stdio_char
 * funcs: fgetwc feof fopen fclose fwrite
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fgetwc (WEOF is 0xFFFF)
 * note:  PROBE: WEOF == (wint_t)0xFFFF on Windows, so a genuine U+FFFF data unit read from a "r,ccs=UTF-16LE" stream is
 *        indistinguishable from end of file by return value alone. Expected: fgetwc returns 0xFFFF but feof stays 0.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_chr_neg_fgetwc_ffff_data_ambiguity_probe_" TCS(TC_CRT_VER) ".tmp"

TC_CASE(ffff_is_data)
{
    FILE *f; wint_t c;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    fwrite("\xff\xfe\xff\xff", 1, 4, f); fclose(f);
    f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    c = fgetwc(f);
    CHECK_EQ_U(c, 0xFFFF);
    CHECK_EQ_U(c, WEOF);
    CHECK_EQ_I(feof(f), 0);
    c = fgetwc(f);
    CHECK_EQ_U(c, WEOF);
    CHECK(feof(f) != 0);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(ffff_is_data); return tc_end(); }
