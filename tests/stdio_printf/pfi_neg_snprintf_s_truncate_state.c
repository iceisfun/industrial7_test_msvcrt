/* TC
 * id:    pfi_neg_snprintf_s_truncate_state
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snprintf_s_l, _snwprintf_s: _TRUNCATE and count semantics
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(truncate_flag)
{
    char b[8]; int n, i; volatile int v = 1234567;
    for (i = 0; i < 8; i++) b[i] = 'Z';
    tc_iph_count = 0;
    n = _snprintf_s(b, 5, _TRUNCATE, "%d", v);
    CHECK_EQ_I(n, -1); CHECK_STR(b, "1234"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_below_output)
{
    char b[8]; int n; volatile int v = 123456;
    tc_iph_count = 0;
    n = _snprintf_s(b, 8, 3, "%d", v);
    CHECK_EQ_I(n, -1); CHECK_STR(b, "123"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_ge_size_overflow)
{
    char b[4]; int n; volatile int v = 123456;
    b[0] = 'x'; errno = 0; tc_iph_count = 0;
    n = _snprintf_s(b, 4, 10, "%d", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(empty_output)
{
    char b[1]; int n; volatile int v = 0;
    b[0] = 'x'; tc_iph_count = 0;
    n = _snprintf_s(b, 1, 1, "%.0d", v);
    CHECK_EQ_I(n, 0); CHECK_STR(b, ""); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(truncate_flag); TC_RUN(count_below_output); TC_RUN(count_ge_size_overflow); TC_RUN(empty_output);
    return tc_end();
}
