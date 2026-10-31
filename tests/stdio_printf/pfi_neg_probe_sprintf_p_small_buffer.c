/* TC
 * id:    pfi_neg_probe_sprintf_p_small_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _sprintf_p
 * note:  Probes that _sprintf_p with a too small count returns -1, calls the invalid-parameter handler once and sets b[0]=0
 *        (the sprintf_s-style behaviour); _sprintf_p docs do not say.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(small_buffer)       { char b[32]; volatile int v = 123456; int r; b[0] = 'X'; tc_iph_count = 0; r = _sprintf_p(b, 3, "%1$d", v); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(small_buffer);
    return tc_end();
}
