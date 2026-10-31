/* TC
 * id:    pfi_neg_pct_conv_int_no_args_nul_terminates
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s _snprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs sprintf_s (buffer[0] set to 0 on failure; invalid parameter handler)
 * note:  Probes: after an invalid format (unknown conversion after %d, trailing lone '%'), the destination buffer's first byte is NUL
 *        and the result is -1. The documented NUL-on-failure is for overflow; for format errors this is what is probed.
 */
#include "tc.h"
#include <stdio.h>

static void fill(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = 'X'; }

TC_CASE(bad_conv)
{
    char b[32]; volatile int v = 1; int r;
    fill(b, 32); tc_iph_count = 0;
    r = sprintf_s(b, sizeof b, "%d %y", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(trailing_percent)
{
    char b[32]; volatile int v = 1; int r;
    fill(b, 32); tc_iph_count = 0;
    r = sprintf_s(b, sizeof b, "%d%", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(snprintf_s_bad)
{
    char b[32]; volatile int v = 1; int r;
    fill(b, 32); tc_iph_count = 0;
    r = _snprintf_s(b, sizeof b, 10, "%d %y", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(bad_conv); TC_RUN(trailing_percent); TC_RUN(snprintf_s_bad);
    return tc_end();
}
