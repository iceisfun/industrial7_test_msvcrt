/* TC
 * id:    pfu_probe_snprintf_s_count_zero
 * area:  stdio_printf
 * funcs: _snprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf_s (count == 0 with a valid buffer is not discussed)
 * note:  PROBING: _snprintf_s(buf, 32, 0, ...) with a valid buffer: empty output -> returns 0, buf[0]==0; non-empty output -> truncated by count: returns -1, buf[0]==0, no invalid-parameter handler.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(count0_empty)
{
    char b[4]; volatile size_t c = 0; volatile const char *f = ""; int n; b[0] = 'x';
    n = _snprintf_s(b, 4, c, (const char *)f);
    CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0);
}
TC_CASE(count0_nonempty)
{
    char b[32]; volatile size_t c = 0; volatile const char *s = "abcdef"; int n; b[0] = 'x'; tc_iph_count = 0;
    n = _snprintf_s(b, 32, c, "%s", (const char *)s);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(count0_empty); TC_RUN(count0_nonempty);
    return tc_end();
}
