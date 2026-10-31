/* TC
 * id:    pfp_neg_n_default_disabled
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * procs: _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_printf_count_output: "By default, %n support is disabled"; MS docs printf type field n: if disabled, invalid parameter handler is invoked
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(percent_n)
{
    char b[32]; int n = 0x5a5a5a5a, r; tc_iph_count = 0; errno = 0;
    r = sprintf(b, "ab%n", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(n, 0x5a5a5a5a);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(percent_hn)
{
    char b[32]; short n = 0x5a5a; int r; tc_iph_count = 0; errno = 0;
    r = sprintf(b, "ab%hn", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(n, 0x5a5a);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(percent_n); TC_RUN(percent_hn);
    return tc_end();
}
