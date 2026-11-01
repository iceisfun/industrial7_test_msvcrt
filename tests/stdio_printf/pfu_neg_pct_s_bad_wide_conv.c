/* TC
 * id:    pfu_neg_pct_s_bad_wide_conv
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs printf format specification (%ls, %lc); errno EILSEQ
 * note:  Observed on Windows 10 22H2, "C" locale: sprintf_s with a lone surrogate in %ls returns -1/EILSEQ, but %lc of U+FFFF returns 0 (empty) and
 *        "[%lc]" of U+263A returns 2 ("[]"), both with errno EILSEQ: the unconvertible character is dropped and the call still succeeds.
 *        Deviates from C11 7.21.6.1p14 (encoding error -> negative return) for %lc.
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
    char b[32] = {1}; volatile wint_t c = 0xFFFF; int r;
    errno = 0;
    r = sprintf_s(b, 32, "%lc", c);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(b[0], 0);
}
TC_CASE(lc_above_ff)
{
    char b[32]; volatile wint_t c = 0x263A; int r;
    errno = 0;
    r = sprintf_s(b, 32, "[%lc]", c);
    CHECK_EQ_I(r, 2); CHECK_EQ_I(errno, EILSEQ); CHECK_STR(b, "[]");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(ls_lone_surrogate); TC_RUN(lc_ffff); TC_RUN(lc_above_ff);
    return tc_end();
}
