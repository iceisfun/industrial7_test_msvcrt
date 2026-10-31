/* TC
 * id:    err_neg_param_itoa_s_range
 * area:  errno_param
 * kind:  negative
 * funcs: _itoa_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _itoa_s, _i64toa_s, _ultoa_s
 * note:  The exact error code for a too-small buffer is not asserted here (docs table vs implementation uncertain). msvcr80/90 omitted: mingw import libs lack _itoa_s.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(buffer_too_small)
{
    char b[3] = { 'x', 'x', 'x' }; errno_t r; volatile int v = 12345; tc_iph_count = 0; errno = 0;
    r = _itoa_s(v, b, sizeof b, 10);
    CHECK(r != 0); CHECK_EQ_I(tc_iph_count, 1);   /* exact code (ERANGE vs EINVAL), errno, buffer: see err_neg_param_itoa_s_small_probe */
}
TC_CASE(radix_1)
{
    char b[34]; errno_t r; volatile int rx = 1; tc_iph_count = 0; errno = 0;
    r = _itoa_s(5, b, sizeof b, rx);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(radix_37)
{
    char b[34]; errno_t r; volatile int rx = 37; tc_iph_count = 0; errno = 0;
    r = _itoa_s(5, b, sizeof b, rx);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buf)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = _itoa_s(5, NULL, 10, 10);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(exact_fit_ok)
{
    char b[6]; errno_t r; volatile int v = 12345; tc_iph_count = 0;
    r = _itoa_s(v, b, sizeof b, 10);
    CHECK_EQ_I(r, 0); CHECK_STR(b, "12345"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no handler API\n"); return 1; }
    TC_RUN(buffer_too_small); TC_RUN(radix_1); TC_RUN(radix_37); TC_RUN(null_buf); TC_RUN(exact_fit_ok);
    return tc_end();
}
