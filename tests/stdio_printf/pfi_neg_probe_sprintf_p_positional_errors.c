/* TC
 * id:    pfi_neg_probe_sprintf_p_positional_errors
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "printf_p positional parameters" (positions 1..N, may not be mixed with non-positional)
 * note:  Probes: (a) "%2$d" with position 1 never referenced, (b) "%0$d" position zero, (c) mixing "%1$d %d". All expected -1 with exactly one handler call.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(missing_position_1) { char b[32]; int r; tc_iph_count = 0; r = _sprintf_p(b, 32, "%2$d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(position_zero)      { char b[32]; int r; tc_iph_count = 0; r = _sprintf_p(b, 32, "%0$d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(mixed)              { char b[32]; int r; tc_iph_count = 0; r = _sprintf_p(b, 32, "%1$d %d", 1, 2); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(missing_position_1); TC_RUN(position_zero); TC_RUN(mixed);
    return tc_end();
}
