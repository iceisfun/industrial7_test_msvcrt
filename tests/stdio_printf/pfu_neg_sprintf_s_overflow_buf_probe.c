/* TC
 * id:    pfu_neg_sprintf_s_overflow_buf_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s _snprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sprintf_s (silent on buffer contents after overflow)
 * note:  PROBING: buffer[0] == 0 after sprintf_s / _snprintf_s fail with a too-small buffer (invalid-parameter handler returning).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(sprintf_s_small)
{
    char b[16]; volatile const char *s = "abcd"; int n, i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    n = sprintf_s(b, 4, "%s", (const char *)s);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(sprintf_s_size_one)
{
    char b[4]; volatile const char *s = "x"; int n; b[0] = 'q';
    n = sprintf_s(b, 1, "%s", (const char *)s);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(snprintf_s_count_ge_size)
{
    char b[16]; volatile size_t sz = 4, c = 10; int n, i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    n = _snprintf_s(b, sz, c, "%s", "abcdef");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    TC_RUN(sprintf_s_small); TC_RUN(sprintf_s_size_one); TC_RUN(snprintf_s_count_ge_size);
    return tc_end();
}
