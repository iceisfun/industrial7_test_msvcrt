/* TC
 * id:    pfu_neg_probe_fprintf_s_lc_ffff
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s
 * crt:   >=140
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf %lc (wcrtomb failure -> EILSEQ)
 * note:  PROBE: fprintf_s("%lc", 0xFFFF) to a narrow stream in the "C" locale returns 0 (nothing written) with errno EILSEQ
 *        (U+FFFF is above 0xFF, unencodable in the C locale). OBSERVED on Win10 22H2 19045 (the return is 0, not -1).
 *        A (test bug): observed n == 0, errno == EILSEQ.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define TMP "tc_pfu_lc_ffff.tmp"

TC_CASE(lc_ffff)
{
    FILE *f; int n; volatile wchar_t wc = 0xFFFF;
    f = fopen(TMP, "w"); CHECK(f != NULL); if (!f) return;
    errno = 0;
    n = fprintf_s(f, "%lc", (wint_t)wc);
    CHECK_EQ_I(n, 0);
    CHECK_EQ_I(errno, EILSEQ);
    fclose(f); remove(TMP);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(lc_ffff); return tc_end(); }
