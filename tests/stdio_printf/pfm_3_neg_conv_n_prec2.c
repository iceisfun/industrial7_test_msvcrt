/* TC
 * id:    pfm_3_neg_conv_n_prec2
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _get_printf_count_output
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs printf type field n; _set_printf_count_output; Parameter validation
 * note:  Continuation of pfm_3_neg_conv_n_flags_width_prec: precision forms %.*n, %.n, %*.*n, %-5.2n and text before the %n. Handler
 *        runs once, int target untouched. Skips when %n is enabled.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
static int enabled;
static const char * volatile vf;
#define SENT 0x5A5A1234

TC_CASE(star_prec) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "%.*n"; sprintf(b, vf, 2, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }
TC_CASE(bare_dot) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "%.n"; sprintf(b, vf, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }
TC_CASE(star_both) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "%*.*n"; sprintf(b, vf, 6, 2, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }
TC_CASE(combo) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "%-5.2n"; sprintf(b, vf, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }
TC_CASE(after_text) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "abc%+5n"; sprintf(b, vf, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_get_printf_count_output);
    enabled = tc_p__get_printf_count_output && tc_p__get_printf_count_output() != 0;
    TC_RUN(star_prec); TC_RUN(bare_dot); TC_RUN(star_both); TC_RUN(combo); TC_RUN(after_text);
    return tc_end();
}
