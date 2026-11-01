/* TC
 * id:    pff_neg_invalid_conversion_after_float
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf format specification (unknown-char behaviour unproven)
 * note:  Unknown conversion characters (%y %! %q) after a valid float spec, in the NON-_s sprintf.
 *        Observed Win10 22H2 ucrtbase: no validation in the plain printf family: the '%' is dropped, the char is copied literally, handler not called,
 *        return = length written ("1.500000y" = 9). (The _s family does validate: see pfv_neg_probe_sprintf_s_format_null_buf0.)
 *        Raw evidence (baseline): TC FAIL pff_neg_invalid_conversion_after_float::pct_y ... try(f) == -1 got=9 want=-1 ; tc_iph_count == 1 got=0
 *        crt narrowed to ucrtbase: the observed behaviour was measured on Win10 22H2 ucrtbase only (older CRTs [unverified]).
 */
#include "tc.h"
#include <stdio.h>

static char g_b[64];
static int try(const char *fmt)
{
    char *b = g_b; volatile double v = 1.5; int r;
    tc_iph_count = 0;
    r = sprintf(b, fmt, v, 2);
    return r;
}
TC_CASE(pct_y)  { const char *volatile f = "%f%y"; CHECK_EQ_I(try(f), 9); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(g_b, "1.500000y"); }
TC_CASE(pct_bang) { const char *volatile f = "%f%!"; CHECK_EQ_I(try(f), 9); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(g_b, "1.500000!"); }
TC_CASE(pct_q)  { const char *volatile f = "%f%q"; CHECK_EQ_I(try(f), 9); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(g_b, "1.500000q"); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(pct_y); TC_RUN(pct_bang); TC_RUN(pct_q);
    return tc_end();
}
