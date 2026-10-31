/* TC
 * id:    pfm_2_neg_conv_percent_trailing
 * area:  stdio_printf
 * funcs: sprintf_s _snprintf
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf format specification / sprintf_s: incomplete or invalid format string -> invalid parameter handler, EINVAL, -1
 * note:  Truncated specifications ("abc%", "%5", "%", "x%.5") end inside a conversion specification; _snprintf (non-_s) is probed in pfm_2_neg_conv_percent_trailing_snprintf_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static int try_s(const char *volatile f)
{
    char b[32]; int r;
    tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, sizeof b, f);
    return r;
}
TC_CASE(abc_pct) { CHECK_EQ_I(try_s("abc%"), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(pct5)    { CHECK_EQ_I(try_s("%5"), -1);   CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(lone)    { CHECK_EQ_I(try_s("%"), -1);    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(pct_dot_num) { CHECK_EQ_I(try_s("x%.5"), -1);  CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(abc_pct); TC_RUN(pct5); TC_RUN(lone); TC_RUN(pct_dot_num);
    return tc_end();
}
