/* TC
 * id:    pfp_neg_sprintf_p_positional_invalid
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "printf_p Positional Parameters"
 * note:  PROBE: invalid positional-parameter formats given to _sprintf_p (position 0, gap in positions,
 *        mixing positional and non positional conversions) are expected to return -1 and invoke the
 *        invalid-parameter handler once. Docs only say positions must be 1..N (_ARGMAX) and consistent.
 *        The positive control (valid reordering) is in its own case.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(probe_pos_zero)
{
    char b[64]; int r; volatile int v = 5;
    tc_iph_count = 0;
    r = _sprintf_p(b, sizeof b, "%0$d", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(probe_gap)
{
    char b[64]; int r; volatile int v = 5, w = 6;
    tc_iph_count = 0;
    r = _sprintf_p(b, sizeof b, "%2$d", v, w);   /* argument 1 never referenced */
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(probe_mixed)
{
    char b[64]; int r; volatile int v = 5, w = 6;
    tc_iph_count = 0;
    r = _sprintf_p(b, sizeof b, "%1$d %d", v, w);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(probe_ptr_positional_ok)
{
    char b[64]; int r; volatile int v = 1, w = 2;
    tc_iph_count = 0;
    r = _sprintf_p(b, sizeof b, "%2$d %1$d", v, w);
    CHECK_EQ_I(r, 3); CHECK_STR(b, "2 1"); CHECK_EQ_I(tc_iph_count, 0);
    if (TC_X64) {
        void *volatile p1 = (void *)(ULONG_PTR)0x1, *volatile p2 = (void *)(ULONG_PTR)0xABC;
        r = _sprintf_p(b, sizeof b, "%2$p %1$p", p1, p2);
        CHECK_EQ_I(r, 33); CHECK_STR(b, "0000000000000ABC 0000000000000001"); CHECK_EQ_I(tc_iph_count, 0);
    }
}

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(probe_pos_zero); TC_RUN(probe_gap); TC_RUN(probe_mixed); TC_RUN(probe_ptr_positional_ok);
    return tc_end();
}
