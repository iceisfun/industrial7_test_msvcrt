/* TC
 * id:    pfu_neg_probe_fwprintf_s_hs_highbyte
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf_s
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fwprintf_s; %hs (narrow string converted with mbrtowc)
 * note:  PROBE: in the "C" locale, fwprintf_s("%hs") of the narrow string "\xE9" on a stream opened with fopen (MS CRT has no stream orientation; the wide text is converted to narrow on output). Probed: either the
 *        byte maps to U+00E9 (return 1) or the conversion fails (-1, EILSEQ). Asserts only that the result is one of those two
 *        and that errno is EILSEQ exactly when the result is -1, and that the stream is usable afterwards.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define TMP "tc_pfu_hs_hb.tmp"

TC_CASE(hs_highbyte)
{
    FILE *f; int n; static volatile char s[2] = { (char)0xE9, 0 };
    f = fopen(TMP, "w"); CHECK(f != NULL); if (!f) return;
    errno = 0;
    n = fwprintf_s(f, L"%hs", (const char *)s);
    CHECK(n == 1 || n == -1);
    if (n == -1) CHECK_EQ_I(errno, EILSEQ);
    errno = 0;
    n = fwprintf_s(f, L"ok");
    CHECK_EQ_I(n, 2);
    fclose(f); remove(TMP);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(hs_highbyte); return tc_end(); }
