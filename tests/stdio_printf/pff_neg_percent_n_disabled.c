/* TC
 * id:    pff_neg_percent_n_disabled
 * area:  stdio_printf
 * funcs: sprintf _get_printf_count_output
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs printf %n: disabled by default, invokes the invalid parameter handler
 * note:  n must stay at its sentinel
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(percent_n_rejected)
{
    char b[64]; int r; volatile int n = -99; volatile double v = 1.5;
    tc_iph_count = 0;
    r = sprintf(b, "%.2f%n", v, (int *)&n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, -99);
}
TC_CASE(count_output_off)
{
    CHECK_EQ_I(_get_printf_count_output(), 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(percent_n_rejected); TC_RUN(count_output_off);
    return tc_end();
}
