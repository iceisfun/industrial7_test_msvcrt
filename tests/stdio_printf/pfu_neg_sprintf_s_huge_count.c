/* TC
 * id:    pfu_neg_sprintf_s_huge_count
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s _snwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf_s (count: maximum number of characters to write; count >= size and output too long: invalid parameter)
 * note:  Probing exactly: count=(size_t)-2 (not _TRUNCATE) acts as "larger than any buffer": short output succeeds, long output hits the handler (-1) rather than truncating. A huge sizeOfBuffer is deliberately not tested.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(narrow_short_ok)
{
    char b[16]; volatile size_t n = 8, c = (size_t)-2; int r;
    tc_iph_count = 0;
    r = _snprintf_s(b, n, c, "%s", "abc");
    CHECK_EQ_I(r, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(narrow_long_handler)
{
    char b[16]; volatile size_t n = 8, c = (size_t)-2; int r;
    b[0] = 'Q'; tc_iph_count = 0;
    r = _snprintf_s(b, n, c, "%s", "abcdefghij");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(wide_short_ok)
{
    wchar_t b[16]; volatile size_t n = 8, c = (size_t)-2; int r;
    tc_iph_count = 0;
    r = _snwprintf_s(b, n, c, L"%ls", L"abc");
    CHECK_EQ_I(r, 3); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_long_handler)
{
    wchar_t b[16]; volatile size_t n = 8, c = (size_t)-2; int r;
    b[0] = L'Q'; tc_iph_count = 0;
    r = _snwprintf_s(b, n, c, L"%ls", L"abcdefghij");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(narrow_short_ok); TC_RUN(narrow_long_handler); TC_RUN(wide_short_ok); TC_RUN(wide_long_handler);
    return tc_end();
}
