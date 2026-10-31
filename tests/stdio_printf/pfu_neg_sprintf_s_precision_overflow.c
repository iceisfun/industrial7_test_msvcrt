/* TC
 * id:    pfu_neg_sprintf_s_precision_overflow
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s _snprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s (buffer too small: handler, buffer[0]=0, returns -1), _snprintf_s (_TRUNCATE), C11 7.21.6.1 (precision of %s is a maximum)
 * note:  Output wider than the buffer because of width/precision. A handler is installed.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(width_too_wide)
{
    char b[16]; volatile size_t n = 8; int r;
    b[0] = 'Q'; tc_iph_count = 0;
    r = sprintf_s(b, n, "%20d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(precision_is_max)
{
    char b[16]; volatile size_t n = 8; int r;
    tc_iph_count = 0;
    r = sprintf_s(b, n, "%.20s", "x");
    CHECK_EQ_I(r, 1); CHECK_STR(b, "x"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(truncate_spaces)
{
    char b[16]; volatile size_t n = 8; int r;
    tc_iph_count = 0;
    r = _snprintf_s(b, n, (size_t)-1, "%20d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "       ");
}
TC_CASE(star_width_huge)
{
    char b[16]; volatile size_t n = 8; volatile int w = 100000; int r;
    b[0] = 'Q'; tc_iph_count = 0;
    r = sprintf_s(b, n, "%*d", w, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(precision_zero)
{
    char b[16]; volatile size_t n = 8; int r;
    b[0] = 'Q';
    r = sprintf_s(b, n, "%.0s", "abc");
    CHECK_EQ_I(r, 0); CHECK_STR(b, "");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(width_too_wide); TC_RUN(precision_is_max); TC_RUN(truncate_spaces);
    TC_RUN(star_width_huge); TC_RUN(precision_zero);
    return tc_end();
}
