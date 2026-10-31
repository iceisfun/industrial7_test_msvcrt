/* TC
 * id:    chr_neg_getw_putw_wrong_mode
 * area:  stdio_char
 * funcs: _getw _putw
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _getw, _putw
 * note:  Wrong-direction streams: EOF (documented) and error indicator set (not explicitly documented, hence inferred).
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
#define FN "tc_chr_neg_getw_putw_wrong_mode_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(getw_on_w) { FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return; CHECK_EQ_I(_getw(f), EOF); CHECK(ferror(f) != 0); fclose(f); remove(FN); }
TC_CASE(putw_on_r)
{
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return; fwrite("abcdefgh", 1, 8, f); fclose(f);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_putw(1, f), EOF); CHECK(ferror(f) != 0); fclose(f); remove(FN);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(getw_on_w); TC_RUN(putw_on_r); return tc_end(); }
