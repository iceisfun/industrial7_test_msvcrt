/* TC
 * id:    pfu_snprintf_s_count_basic
 * area:  stdio_printf
 * funcs: _snprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s: count = maximum number of characters to write, not counting the NUL; returns number of bytes written excluding NUL
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(count_gt_len)
{
    char b[16]; volatile size_t c = 10; int n;
    n = _snprintf_s(b, 16, c, "%s", "abc");
    CHECK_EQ_I(n, 3); CHECK_STR(b, "abc");
}
TC_CASE(count_eq_len)
{
    char b[4]; volatile size_t c = 3; int n;
    n = _snprintf_s(b, 4, c, "%s", "abc");
    CHECK_EQ_I(n, 3); CHECK_STR(b, "abc");
}
TC_CASE(count_huge)
{
    char b[64]; volatile size_t c = 1000; int n;
    n = _snprintf_s(b, 64, c, "%d-%d", 12, 34);
    CHECK_EQ_I(n, 5); CHECK_STR(b, "12-34");
}
TC_CASE(precision_within_count)
{
    char b[16]; volatile size_t c = 4; int n;
    n = _snprintf_s(b, 16, c, "%.3s", "abcdef");
    CHECK_EQ_I(n, 3); CHECK_STR(b, "abc");
}
int main(void)
{
    tc_begin();
    TC_RUN(count_gt_len); TC_RUN(count_eq_len); TC_RUN(count_huge); TC_RUN(precision_within_count);
    return tc_end();
}
