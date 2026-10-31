/* TC
 * id:    pfq_probe_sprintf_p_tail_untouched
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _sprintf_p
 * note:  PROBES that a successful _sprintf_p writes only the result plus its NUL and leaves the rest of the buffer untouched
 *        (no debug-style 0xFE fill in the release CRT). Not documented; the debug CRTs do fill, release DLLs are expected not to.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f_d = "%1$d-%2$s";

TC_CASE(tail_untouched)
{
    char b[16]; int i, n;
    for (i = 0; i < 16; i++) b[i] = (char)0xAA;
    n = _sprintf_p(b, sizeof b, f_d, 1, "ab");
    CHECK_EQ_I(n, 4); CHECK_EQ_I(b[4], 0);
    for (i = 5; i < 16; i++) CHECK_EQ_U((unsigned char)b[i], 0xAA);
}

int main(void) { tc_begin(); TC_RUN(tail_untouched); return tc_end(); }
