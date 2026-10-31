/* TC
 * id:    pfu_snprintf_s_count_less_than_size
 * area:  stdio_printf
 * funcs: _snprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s: count = max chars stored (not counting NUL); if truncated by count, returns -1 and NUL-terminates; no invalid parameter handler
 */
#include "tc.h"
#include <stdio.h>

static const char *vol_s(void) { static volatile const char *s = "abcdef"; return (const char *)s; }

TC_CASE(count3)
{
    char b[32]; volatile size_t c = 3; int n; tc_iph_count = 0;
    n = _snprintf_s(b, 32, c, "%s", vol_s());
    CHECK_EQ_I(n, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_eq_len)
{
    char b[32]; volatile size_t c = 6; int n; tc_iph_count = 0;
    n = _snprintf_s(b, 32, c, "%s", vol_s());
    CHECK_EQ_I(n, 6); CHECK_STR(b, "abcdef"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_len_minus_1)
{
    char b[32]; volatile size_t c = 5; int n; tc_iph_count = 0;
    n = _snprintf_s(b, 32, c, "%s", vol_s());
    CHECK_EQ_I(n, -1); CHECK_STR(b, "abcde"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();  /* msvcrt.dll has no handler API; counters then stay 0 */
    TC_RUN(count3); TC_RUN(count_eq_len); TC_RUN(count_len_minus_1);
    return tc_end();
}
