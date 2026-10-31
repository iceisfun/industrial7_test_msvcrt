/* TC
 * id:    pff_neg_invalid_conversion_after_float
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt msvcr80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf format specification (unknown-char behaviour unproven)
 * note:  PROBE: unknown conversion characters (%y %! %q) after a valid float spec invoke the handler and return -1.
 *        MS format-spec docs also say unknown chars after % are treated as ordinary text, so this is unproven. Buffer content not asserted.
 */
#include "tc.h"
#include <stdio.h>

static int try(const char *fmt)
{
    char b[64]; volatile double v = 1.5; int r;
    tc_iph_count = 0;
    r = sprintf(b, fmt, v, 2);
    return r;
}
TC_CASE(pct_y)  { const char *volatile f = "%f%y"; CHECK_EQ_I(try(f), -1); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(pct_bang) { const char *volatile f = "%f%!"; CHECK_EQ_I(try(f), -1); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(pct_q)  { const char *volatile f = "%f%q"; CHECK_EQ_I(try(f), -1); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(pct_y); TC_RUN(pct_bang); TC_RUN(pct_q);
    return tc_end();
}
