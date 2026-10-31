/* TC
 * id:    pfm_2_neg_probe_star_incomplete_spec
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s _snprintf_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs sprintf_s (invalid format string -> invalid parameter handler, return -1); format string ending inside a conversion specification
 * note:  PROBES: a format that ends in the middle of a conversion specification ('%*', '%.*', '%.', '%-') is an invalid format
 *        string for the _s functions: -1 and exactly one invalid-parameter call, and separately (cases b0_*, probe) buffer[0]==0 afterwards.
 *        An int argument is supplied for '*' so no missing-argument read happens. Cases are independent.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile f_star = "%*", *volatile f_dotstar = "%.*", *volatile f_dot = "%.", *volatile f_minus = "%-";
static volatile int four = 4;

#define SPF(fmt) do { char b[16]; int r; b[0] = 'Q'; tc_iph_count = 0; r = sprintf_s(b, sizeof b, fmt, four); \
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); } while (0)
TC_CASE(star_end)    { SPF(f_star); }
TC_CASE(dotstar_end) { SPF(f_dotstar); }
TC_CASE(dot_end)     { SPF(f_dot); }
TC_CASE(minus_end)   { SPF(f_minus); }
TC_CASE(b0_star_end)
{
    char b[16]; b[0] = 'Q'; tc_iph_count = 0;
    (void)sprintf_s(b, sizeof b, f_star, four);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(b0_dot_end)
{
    char b[16]; b[0] = 'Q'; tc_iph_count = 0;
    (void)sprintf_s(b, sizeof b, f_dot, four);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(snprintf_s_star_end)
{
    char b[16]; int r; b[0] = 'Q'; tc_iph_count = 0;
    r = _snprintf_s(b, sizeof b, 8, f_star, four);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(snprintf_s_dot_end)
{
    char b[16]; int r; b[0] = 'Q'; tc_iph_count = 0;
    r = _snprintf_s(b, sizeof b, 8, f_dot, four);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(star_end); TC_RUN(dotstar_end); TC_RUN(dot_end); TC_RUN(minus_end); TC_RUN(b0_star_end); TC_RUN(b0_dot_end); TC_RUN(snprintf_s_star_end); TC_RUN(snprintf_s_dot_end);
    return tc_end();
}
