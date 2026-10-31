/* TC
 * id:    pfp_neg_n_default_disabled_win7
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * procs: _set_invalid_parameter_handler
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_printf_count_output (default disabled since VS2005)
 * note:  PROBE: on the Win7-era CRTs %n is also disabled by default: sprintf returns -1, handler runs, the target is untouched. msvcr80 deliberately not listed.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(percent_n)
{
    char b[32]; int n = 0x5a5a5a5a, r; tc_iph_count = 0;
    r = sprintf(b, "ab%n", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(n, 0x5a5a5a5a); CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(percent_n);
    return tc_end();
}
