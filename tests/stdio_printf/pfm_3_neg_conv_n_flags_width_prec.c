/* TC
 * id:    pfm_3_neg_conv_n_flags_width_prec
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _get_printf_count_output
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs printf type field n ("%n is disabled by default; if encountered the invalid parameter handler is invoked"); _set_printf_count_output; Parameter validation
 * note:  %n with any flag, width or precision is still %n: the invalid-parameter handler runs once and the target int is never written.
 *        Cases skip when the process has %n enabled. Return value / errno are in pfm_3_neg_conv_n_ret_errno_probe.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
static int enabled;
static const char * volatile vf;
#define SENT 0x5A5A1234

TC_CASE(minus) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "%-n"; sprintf(b, vf, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }
TC_CASE(plus) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "%+n"; sprintf(b, vf, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }
TC_CASE(space) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "% n"; sprintf(b, vf, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }
TC_CASE(hash) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "%#n"; sprintf(b, vf, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }
TC_CASE(zero) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "%0n"; sprintf(b, vf, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }
TC_CASE(width) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "%5n"; sprintf(b, vf, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }
TC_CASE(star_width) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "%*n"; sprintf(b, vf, 4, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }
TC_CASE(prec) { char b[48]; int n = SENT; if (enabled) TC_SKIP("%n enabled");
    tc_iph_count = 0; vf = "%.3n"; sprintf(b, vf, &n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_get_printf_count_output);
    enabled = tc_p__get_printf_count_output && tc_p__get_printf_count_output() != 0;
    TC_RUN(minus); TC_RUN(plus); TC_RUN(space); TC_RUN(hash); TC_RUN(zero); TC_RUN(width); TC_RUN(star_width); TC_RUN(prec);
    return tc_end();
}
