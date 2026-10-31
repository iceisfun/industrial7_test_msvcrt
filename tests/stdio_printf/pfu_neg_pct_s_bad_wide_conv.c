/* TC
 * id:    pfu_neg_pct_s_bad_wide_conv
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf format specification (%ls, %lc); errno EILSEQ
 * note:  PROBING exactly: UCRT sprintf_s converting an unconvertible wide character to the narrow buffer in the "C" locale. Wide chars above 0xFF cannot be represented in the C locale, so the call is expected to return -1 with errno == EILSEQ (no invalid-parameter handler assumed; not asserted). Legacy CRTs may silently skip the character, so they are not listed. ASCII path is deliberately not tested here.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(ls_lone_surrogate)
{
    char b[32]; const wchar_t *volatile w = L"a\xD800" L"b"; int r;
    errno = 0;
    r = sprintf_s(b, 32, "%ls", w);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(lc_ffff)
{
    char b[32]; volatile wint_t c = 0xFFFF; int r;
    errno = 0;
    r = sprintf_s(b, 32, "%lc", c);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(lc_above_ff)
{
    char b[32]; volatile wint_t c = 0x263A; int r;
    errno = 0;
    r = sprintf_s(b, 32, "[%lc]", c);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(ls_lone_surrogate); TC_RUN(lc_ffff); TC_RUN(lc_above_ff);
    return tc_end();
}
