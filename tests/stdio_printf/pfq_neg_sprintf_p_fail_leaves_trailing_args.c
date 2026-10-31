/* TC
 * id:    pfq_neg_sprintf_p_fail_leaves_trailing_args
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf_p positional parameters (positions used must form a contiguous 1..N set)
 * note:  PROBE: "%1$d%2$d" with three arguments supplied succeeds (surplus trailing args ignored, as in C), while "%1$d%3$d" (position 2 never used, a gap) fails with -1 and the invalid-parameter handler. Complements pfq_neg_sprintf_p_position_gap by pairing the pass/fail forms with the same argument list.
 */
#include "tc.h"
#include "pfq_shim.h"
#include <errno.h>

static const char * volatile Fok = "%1$d%2$d", * volatile Fgap = "%1$d%3$d", * volatile Fgap2 = "%2$d%3$d";

TC_CASE(surplus_args_ok)
{
    char b[32]; int r;
    tc_iph_count = 0;
    r = pq_sprintf_p(b, sizeof b, Fok, 1, 2, 3);
    CHECK_EQ_I(r, 2); CHECK_STR(b, "12");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(gap_fails)
{
    char b[32]; int r;
    b[0] = 'x'; tc_iph_count = 0; errno = 0;
    r = pq_sprintf_p(b, sizeof b, Fgap, 1, 2, 3);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(missing_first_fails)
{
    char b[32]; int r;
    b[0] = 'x'; tc_iph_count = 0; errno = 0;
    r = pq_sprintf_p(b, sizeof b, Fgap2, 1, 2, 3);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(surplus_args_ok); TC_RUN(gap_fails); TC_RUN(missing_first_fails);
    return tc_end();
}
