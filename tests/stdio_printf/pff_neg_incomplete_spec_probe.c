/* TC
 * id:    pff_neg_incomplete_spec_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf
 * note:  probes: how sprintf (non-_s) treats a format string ending inside a conversion
 *        Observed on Win10 22H2 19045 ucrtbase: the non-_s printf family does not validate format strings (no handler call, no errno); the bad conversion char is copied through.
 *        A (test bug): sprintf with "x%.2", "x%", "%5" returns 1, 1, 0 (text before the dangling spec is kept).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(ends_after_precision)
{
    char b[64]; int r; volatile double v = 1.5; const char *volatile f = "x%.2";
    tc_iph_count = 0; r = sprintf(b, f, v);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "x"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(lone_percent)
{
    char b[64]; int r; const char *volatile f = "x%";
    tc_iph_count = 0; r = sprintf(b, f);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "x"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(ends_after_width)
{
    char b[64]; int r; const char *volatile f = "%5";
    tc_iph_count = 0; r = sprintf(b, f);
    CHECK_EQ_I(r, 0); CHECK_STR(b, ""); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(ends_after_precision); TC_RUN(lone_percent); TC_RUN(ends_after_width);
    return tc_end();
}
