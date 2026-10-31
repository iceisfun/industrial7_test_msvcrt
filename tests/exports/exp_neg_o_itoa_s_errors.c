/* TC
 * id:    exp_neg_o_itoa_s_errors
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: _o__itoa_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs _itoa_s, _ltoa_s, _ultoa_s (EINVAL for NULL buffer/zero size/bad radix, buffer[0] cleared, invalid parameter handler)
 * note:  The exact errno-style code for a too-small buffer is not asserted here; only failure, buffer[0]==0 and one handler call. The _o_ export is undocumented (inferred to match _itoa_s). buffer[0] is not asserted for a bad radix (order of validation vs clearing not documented).
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *itoa_s_t)(int, char *, size_t, int);
static itoa_s_t p_itoa_s;

TC_CASE(bad_radix)
{
    char b[40]; int before;
    if (!p_itoa_s) { CHECK(p_itoa_s != NULL); return; }
    before = tc_iph_count;
    CHECK_EQ_I(p_itoa_s(5, b, sizeof b, 1), EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
    before = tc_iph_count;
    CHECK_EQ_I(p_itoa_s(5, b, sizeof b, 37), EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
TC_CASE(small_buffer)
{
    char b[3]; int before = tc_iph_count;
    if (!p_itoa_s) { CHECK(p_itoa_s != NULL); return; }
    b[0] = 'x';
    CHECK(p_itoa_s(12345, b, 3, 10) != 0);
    CHECK_EQ_I(b[0], 0);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
TC_CASE(null_and_zero)
{
    char b[8]; int before = tc_iph_count;
    if (!p_itoa_s) { CHECK(p_itoa_s != NULL); return; }
    CHECK_EQ_I(p_itoa_s(5, NULL, 8, 10), EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
    before = tc_iph_count;
    CHECK_EQ_I(p_itoa_s(5, b, 0, 10), EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
TC_CASE(valid_radix_control)
{
    char b[40]; int before = tc_iph_count;
    if (!p_itoa_s) { CHECK(p_itoa_s != NULL); return; }
    CHECK_EQ_I(p_itoa_s(5, b, sizeof b, 2), 0);
    CHECK_STR(b, "101");
    CHECK_EQ_I(p_itoa_s(35, b, sizeof b, 36), 0);
    CHECK_STR(b, "z");
    CHECK_EQ_I(tc_iph_count, before);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    p_itoa_s = (itoa_s_t)tc_crt_proc("_o__itoa_s");
    TC_RUN(bad_radix); TC_RUN(small_buffer); TC_RUN(null_and_zero); TC_RUN(valid_radix_control);
    return tc_end();
}
