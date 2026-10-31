/* TC
 * id:    pfi_neg_sprintf_s_overflow_buf_state
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s: on overflow buffer[0]=0, invalid parameter handler, ERANGE
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(overflow)
{
    char b[4]; int n; volatile int v = 12345;
    b[0] = 'x'; errno = 0; tc_iph_count = 0;
    n = sprintf_s(b, 4, "%d", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(width_overflow)
{
    char b[8]; int n; volatile int v = 7;
    b[0] = 'x'; errno = 0; tc_iph_count = 0;
    n = sprintf_s(b, 8, "%10d", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(exact_fit)
{
    char b[4]; int n; volatile int v = 123;
    errno = 0; tc_iph_count = 0;
    n = sprintf_s(b, 4, "%d", v);
    CHECK_EQ_I(n, 3); CHECK_STR(b, "123"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(overflow); TC_RUN(width_overflow); TC_RUN(exact_fit);
    return tc_end();
}
