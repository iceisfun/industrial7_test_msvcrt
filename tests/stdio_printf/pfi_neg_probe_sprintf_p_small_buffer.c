/* TC
 * id:    pfi_neg_probe_sprintf_p_small_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _sprintf_p
 * note:  Probes that _sprintf_p with a too small count returns -2, does not call the invalid-parameter handler and leaves the
 *        partial output in the buffer (b[0]=='1'); _sprintf_p docs do not say.
 *        A (test bug): observed r == -2 (not -1), tc_iph_count == 0, b[0] == '1' on Win10 22H2 19045.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(small_buffer)       { char b[32]; volatile int v = 123456; int r; b[0] = 'X'; tc_iph_count = 0; r = _sprintf_p(b, 3, "%1$d", v); CHECK_EQ_I(r, -2); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(b[0], '1'); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(small_buffer);
    return tc_end();
}
