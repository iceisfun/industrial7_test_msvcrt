/* TC
 * id:    pfq_neg_sprintf_p_small_buffer_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p
 * procs: _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _sprintf_p
 * note:  PROBES what _sprintf_p does when the result does not fit in the count: expected return -1 (not the would-be length) and no write
 *        past the count. Whether the invalid-parameter handler fires and whether buf[0] is NUL are deliberately NOT asserted.
 *        exact_no_nul_room only checks nothing is written past the count. count_zero probes that count 0 returns a negative value.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f = "%1$s%2$s";

TC_CASE(too_small)
{
    char b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = (char)0xAA;
    r = _sprintf_p(b, 4, f, "abc", "def");   /* needs 7 + NUL */
    CHECK_EQ_I(r, -1);
    for (i = 4; i < 16; i++) CHECK_EQ_U((unsigned char)b[i], 0xAA);
}
TC_CASE(exact_no_nul_room)
{
    char b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = (char)0xAA;
    r = _sprintf_p(b, 6, f, "abc", "def");   /* 6 chars, NUL does not fit; return value not asserted */
    (void)r;
    for (i = 6; i < 16; i++) CHECK_EQ_U((unsigned char)b[i], 0xAA);
}
TC_CASE(count_zero)
{
    char b[4]; int r;
    b[0] = 'q';
    r = _sprintf_p(b, 0, f, "a", "b");
    CHECK(r < 0);
    CHECK_EQ_I(b[0], 'q');
}

int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(too_small); TC_RUN(exact_no_nul_room); TC_RUN(count_zero);
    return tc_end();
}
