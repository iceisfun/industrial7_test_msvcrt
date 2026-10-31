/* TC
 * id:    pfr_neg_snprintf_legacy_trunc
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf
 * procs: _set_invalid_parameter_handler
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf: if len > count, count bytes are written, no NUL is appended, return is a negative value (-1)
 * note:  Truncation of the legacy _snprintf is not an invalid-parameter condition.
 */
#include "tc.h"
#include <stdio.h>

#define G 0x5A
TC_CASE(trunc_abcdefgh)
{
    char b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = G;
    r = _snprintf(b, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == 'a' && b[1] == 'b' && b[2] == 'c' && b[3] == 'd');
    CHECK_EQ_I((unsigned char)b[4], G);
}
TC_CASE(literal_format)
{
    char b[16]; int i, r; volatile const char *f = "abcdefgh";
    for (i = 0; i < 16; i++) b[i] = G;
    r = _snprintf(b, 4, (const char *)f);
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == 'a' && b[3] == 'd');
    CHECK_EQ_I((unsigned char)b[4], G);
}
TC_CASE(count_one)
{
    char b[8]; int i, r;
    for (i = 0; i < 8; i++) b[i] = G;
    r = _snprintf(b, 1, "%s", "abc");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(b[0], 'a');
    CHECK_EQ_I((unsigned char)b[1], G);
}
TC_CASE(handler_not_invoked)
{
    char b[8]; int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    r = _snprintf(b, 2, "%s", "abcdef");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(trunc_abcdefgh); TC_RUN(literal_format); TC_RUN(count_one); TC_RUN(handler_not_invoked);
    return tc_end();
}
