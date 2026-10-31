/* TC
 * id:    pfp_neg_n_enabled_sprintf_s_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s _set_printf_count_output _get_printf_count_output
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_printf_count_output, printf format specification (%n); the claim that _s variants reject %n even when enabled is NOT documented here (probe)
 * note:  PROBE: with _set_printf_count_output(1) the non-secure printf family honours %n, but sprintf_s is
 *        believed to keep rejecting it (-1, handler once, target untouched). The global flag is restored to 0.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(probe_s_variant_rejects)
{
    char b[32]; int r, old; volatile int n = -99;
    old = _set_printf_count_output(1);
    tc_iph_count = 0;
    r = sprintf_s(b, sizeof b, "ab%n", (int *)&n);
    _set_printf_count_output(old);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, -99);
}
TC_CASE(flag_restored)
{
    CHECK_EQ_I(_get_printf_count_output(), 0);
}

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(probe_s_variant_rejects); TC_RUN(flag_restored);
    return tc_end();
}
