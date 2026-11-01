/* TC
 * id:    pfp_neg_pct_decorated_sprintf_s_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   >=140
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf format specification (type field '%'); C11 7.21.6.1p9 (%% must be exactly "%%")
 * note:  PROBE: behaviour of '%' with width, precision, or size prefix between the two percent signs.
 *        C says the sequence must be exactly '%%'; OBSERVED: sprintf_s rejects '%5%', '%.3%' and '%ll%' alike (-1, handler once, buf[0]==0). Each format is its own case
 *        asserting exactly one outcome, so a wrong guess shows precisely what Windows does.
 *        A (test bug): observed sprintf_s("%5%") and ("%.3%") return -1 with iph 1 and an empty buffer on Win10 22H2 19045 (not a literal '%').
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(probe_w5)
{
    char b[32]; int r; const char *volatile f = "%5%";
    b[0] = 'x'; tc_iph_count = 0;
    r = sprintf_s(b, sizeof b, f);
    CHECK_EQ_I(r, -1); CHECK_STR(b, ""); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(probe_prec)
{
    char b[32]; int r; const char *volatile f = "%.3%";
    b[0] = 'x'; tc_iph_count = 0;
    r = sprintf_s(b, sizeof b, f);
    CHECK_EQ_I(r, -1); CHECK_STR(b, ""); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(probe_ll_pct)
{
    char b[32]; int r; const char *volatile f = "%ll%";
    b[0] = 'x'; tc_iph_count = 0;
    r = sprintf_s(b, sizeof b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(plain_pct_control)
{
    char b[32]; int r; const char *volatile f = "100%%";
    r = sprintf_s(b, sizeof b, f);
    CHECK_EQ_I(r, 4); CHECK_STR(b, "100%");
}

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(plain_pct_control); TC_RUN(probe_w5); TC_RUN(probe_prec); TC_RUN(probe_ll_pct);
    return tc_end();
}
