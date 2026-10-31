/* TC
 * id:    pff_neg_incomplete_spec_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf
 * note:  probes: UCRT treats a format string ending inside a conversion as an invalid parameter
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(ends_after_precision)
{
    char b[64]; int r; volatile double v = 1.5; const char *volatile f = "x%.2";
    tc_iph_count = 0; r = sprintf(b, f, v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(lone_percent)
{
    char b[64]; int r; const char *volatile f = "x%";
    tc_iph_count = 0; r = sprintf(b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(ends_after_width)
{
    char b[64]; int r; const char *volatile f = "%5";
    tc_iph_count = 0; r = sprintf(b, f);
    CHECK_EQ_I(r, -1);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(ends_after_precision); TC_RUN(lone_percent); TC_RUN(ends_after_width);
    return tc_end();
}
