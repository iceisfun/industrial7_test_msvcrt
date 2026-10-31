/* TC
 * id:    pfu_neg_sprintf_s_buffer_too_small
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s: formatted string too long for buffer -> invalid parameter handler, returns -1
 * note:  buffer[0]==0 after failure is probed in pfu_neg_sprintf_s_overflow_buf_probe; bytes at index >= sizeOfBuffer must stay untouched.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(one_short)
{
    char b[16]; volatile const char *s = "abcd"; int n, i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0;
    n = sprintf_s(b, 4, "%s", (const char *)s);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
    for (i = 4; i < 16; i++) CHECK_EQ_I(b[i], 'Z');
}
TC_CASE(exact_fit_ok)
{
    char b[5]; volatile const char *s = "abcd"; int n;
    tc_iph_count = 0;
    n = sprintf_s(b, 5, "%s", (const char *)s);
    CHECK_EQ_I(n, 4); CHECK_STR(b, "abcd"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(size_one)
{
    char b[4]; volatile const char *s = "x"; int n; b[0] = 'q';
    tc_iph_count = 0;
    n = sprintf_s(b, 1, "%s", (const char *)s);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    TC_RUN(one_short); TC_RUN(exact_fit_ok); TC_RUN(size_one);
    return tc_end();
}
