/* TC
 * id:    pfv_neg_positional_mixed
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf_p (positions start at 1)
 * note:  PROBE: "%1$s %s" (mixed) and "%0$s" invoke the invalid parameter handler and return -1.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(mixed_invalid)
{
    char b[64]; volatile const char *s = "a"; int n; tc_iph_count = 0;
    n = _sprintf_p(b, 64, "%1$s %s", s, s);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(position_zero_invalid)
{
    char b[64]; volatile const char *s = "a"; int n; tc_iph_count = 0;
    n = _sprintf_p(b, 64, "%0$s", s);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(mixed_invalid); TC_RUN(position_zero_invalid); return tc_end(); }
