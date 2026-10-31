/* TC
 * id:    stl_neg_itoa_s_small_buffer
 * area:  stdlib
 * funcs:
 * procs: _itoa_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa_s, _i64toa_s (EINVAL for NULL buffer / size 0 / bad radix, ERANGE for too small)
 * note:  The ERANGE case asserts only the return value (not the handler count). The buffer's state after failure is
 *        not asserted here; see stl_neg_itoa_s_buffer_state_probe.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

TC_DECL_PROC(errno_t, _itoa_s, (int, char *, size_t, int));

TC_CASE(too_small)
{
    char b[8]; errno_t r;
    memset(b, 'x', sizeof b);
    tc_iph_install();
    r = tc_p__itoa_s(12345, b, 3, 10);
    CHECK_EQ_I(r, ERANGE);
}
TC_CASE(size_zero)
{
    char b[8]; errno_t r;
    memset(b, 'x', sizeof b);
    tc_iph_install(); tc_iph_count = 0;
    r = tc_p__itoa_s(5, b, 0, 10);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buf)
{
    errno_t r; tc_iph_install(); tc_iph_count = 0;
    r = tc_p__itoa_s(5, NULL, 10, 10);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(bad_radix)
{
    char b[8]; errno_t r;
    memset(b, 'x', sizeof b);
    tc_iph_install(); tc_iph_count = 0;
    r = tc_p__itoa_s(5, b, sizeof b, 1);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    r = tc_p__itoa_s(5, b, sizeof b, 37);
    CHECK_EQ_I(r, EINVAL);
}
TC_CASE(exact_fit)
{
    char b[8]; errno_t r;
    memset(b, 'x', sizeof b);
    tc_iph_install(); tc_iph_count = 0;
    r = tc_p__itoa_s(12345, b, 6, 10);
    CHECK_EQ_I(r, 0);
    CHECK_STR(b, "12345");
    r = tc_p__itoa_s(-12, b, 4, 10);
    CHECK_EQ_I(r, 0);
    CHECK_STR(b, "-12");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_itoa_s);
    if (!tc_p__itoa_s) { tc_write("TC FAIL stl_neg_itoa_s_small_buffer::__import _itoa_s missing\n"); return 1; }
    TC_RUN(too_small); TC_RUN(size_zero); TC_RUN(null_buf); TC_RUN(bad_radix); TC_RUN(exact_fit);
    return tc_end();
}
