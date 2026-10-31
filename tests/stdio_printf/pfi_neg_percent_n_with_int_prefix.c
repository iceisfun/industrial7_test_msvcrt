/* TC
 * id:    pfi_neg_percent_n_with_int_prefix
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf _set_printf_count_output _get_printf_count_output
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf %n; _set_printf_count_output; _get_printf_count_output (disabled by default)
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(default_off) { CHECK_EQ_I(_get_printf_count_output(), 0); }
TC_CASE(rejected_when_off)
{
    char b[64]; volatile int n = -99; volatile int v = 12; int r;
    tc_iph_count = 0;
    r = sprintf(b, "%d%n", v, (int *)&n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, -99);
}
TC_CASE(hn_ln_rejected_when_off)
{
    char b[64]; volatile short sn = -9; volatile long ln = -9; volatile int v = 1; int r;
    tc_iph_count = 0;
    r = sprintf(b, "%d%hn", v, (short *)&sn);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(sn, -9);
    tc_iph_count = 0;
    r = sprintf(b, "%d%ln", v, (long *)&ln);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(ln, -9);
}
TC_CASE(enabled_stores_count)
{
    char b[64]; volatile int n = -99; volatile int v = 42; int r, prev;
    prev = _set_printf_count_output(1);
    CHECK_EQ_I(prev, 0);
    CHECK_EQ_I(_get_printf_count_output(), 1);
    tc_iph_count = 0;
    r = sprintf(b, "%05d%n", v, (int *)&n);
    CHECK_EQ_I(r, 5); CHECK_EQ_I(n, 5); CHECK_STR(b, "00042"); CHECK_EQ_I(tc_iph_count, 0);
    _set_printf_count_output(prev);
    CHECK_EQ_I(_get_printf_count_output(), 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(default_off); TC_RUN(rejected_when_off); TC_RUN(hn_ln_rejected_when_off); TC_RUN(enabled_stores_count);
    return tc_end();
}
