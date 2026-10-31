/* TC
 * id:    pfp_neg_n_in_sprintf_s
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_printf_count_output (%n disabled by default); sprintf_s family invokes the invalid parameter handler on invalid format
 * note:  Asserts the handler ran, a -1 return and that the target was not written.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(percent_n)
{
    char b[32]; int n = 0x5a5a5a5a, r; tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, sizeof b, "a%n", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x5a5a5a5a); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(percent_n);
    return tc_end();
}
