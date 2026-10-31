/* TC
 * id:    pfu_neg_fprintf_s_ls_unencodable
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fprintf_s, printf type field %ls (EILSEQ on encoding error is not cited verbatim; C11 7.29.6.1 / 7.21.6.1)
 * note:  PROBE (downgraded from documented: the exact EILSEQ/-1 contract for %ls is not cited from MS docs). Stream variant of the sprintf_s EILSEQ case. In the "C" locale a wide char above 0xFF (lone surrogate U+D800; U+FFFF is in pfu_neg_probe_fprintf_s_lc_ffff)
 *        cannot be converted to a narrow char, so fprintf_s returns -1 with errno EILSEQ. Output written before the failure is
 *        not asserted. The stream is expected to remain usable (ferror is not asserted).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define TMP "tc_pfu_ls_unenc.tmp"

TC_CASE(ls_lone_surrogate)
{
    FILE *f; int n; volatile wchar_t wc = 0xD800; wchar_t ws[2];
    ws[0] = wc; ws[1] = 0;
    f = fopen(TMP, "w"); CHECK(f != NULL); if (!f) return;
    errno = 0;
    n = fprintf_s(f, "%ls", ws);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(errno, EILSEQ);
    fclose(f); remove(TMP);
}
TC_CASE(stream_usable_after)
{
    FILE *f; int n; char buf[64]; size_t got; volatile wchar_t wc = 0xD800; wchar_t ws[2];
    ws[0] = wc; ws[1] = 0;
    f = fopen(TMP, "w"); CHECK(f != NULL); if (!f) return;
    n = fprintf_s(f, "%ls", ws);
    CHECK_EQ_I(n, -1);
    n = fprintf_s(f, "ok");
    CHECK_EQ_I(n, 2);
    CHECK_EQ_I(fclose(f), 0);
    f = fopen(TMP, "rb"); CHECK(f != NULL);
    if (f) {
        got = fread(buf, 1, sizeof buf - 1, f); buf[got] = 0; fclose(f);
        CHECK(got >= 2);
        if (got >= 2) CHECK_STR(buf + got - 2, "ok");
    }
    remove(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(ls_lone_surrogate); TC_RUN(stream_usable_after);
    return tc_end();
}
