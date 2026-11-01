/* TC
 * id:    pfv_neg_probe_incomplete_spec_end
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf _snprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs printf format specification (a specification must end in a conversion character)
 * note:  Observed on Windows 10 22H2 ucrtbase: a non-secure sprintf/_snprintf format that ends inside a specification ("%l", "%ll", "%5", "%-.", "%h", "%+")
 *        is silently accepted: returns 0 (nothing printed), errno 0, no handler call.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f_l = "%l", *volatile f_ll = "%ll", *volatile f_5 = "%5", *volatile f_dot = "%-.", *volatile f_h = "%h", *volatile f_plus = "%+";

static void chk(const char *f)
{
    char b[32]; int r; errno = 0; tc_iph_count = 0;
    r = sprintf(b, f);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(pct_l) { chk(f_l); }
TC_CASE(pct_ll) { chk(f_ll); }
TC_CASE(pct_width_only) { chk(f_5); }
TC_CASE(pct_dot_only) { chk(f_dot); }
TC_CASE(pct_h) { chk(f_h); }
TC_CASE(pct_flags_only) { chk(f_plus); }
TC_CASE(snprintf_pct_ll)
{
    char b[32]; int r; errno = 0; tc_iph_count = 0;
    r = _snprintf(b, sizeof b, f_ll);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(pct_l); TC_RUN(pct_ll); TC_RUN(pct_width_only); TC_RUN(pct_dot_only); TC_RUN(pct_h); TC_RUN(pct_flags_only); TC_RUN(snprintf_pct_ll);
    return tc_end();
}
