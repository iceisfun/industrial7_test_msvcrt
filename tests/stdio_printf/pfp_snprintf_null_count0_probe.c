/* TC
 * id:    pfp_snprintf_null_count0_probe
 * area:  stdio_printf
 * funcs: _snprintf, _scprintf
 * procs: _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _snprintf (legacy: negative return when output does not fit); _scprintf
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice) 1: _snprintf(NULL,0,"abc") returns 3, the needed length, with no invalid-parameter call (old guess -1 was wrong).
 *        PROBE 2 (unchanged):
 *        PROBE 2: _scprintf("%n") with %n disabled returns -1 and calls the invalid parameter handler.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(snprintf_null_count0_ret)
{
    char *volatile b = NULL; int r;
    tc_iph_count = 0;
    r = _snprintf(b, 0, "abc");
    CHECK_EQ_I(r, 3);
}
TC_CASE(snprintf_null_count0_no_iph)
{
    char *volatile b = NULL;
    tc_iph_count = 0;
    (void)_snprintf(b, 0, "abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(scprintf_n_disabled)
{
    int n = 0x5a5a, r; tc_iph_count = 0; errno = 0;
    r = _scprintf("ab%n", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x5a5a);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(snprintf_null_count0_ret); TC_RUN(snprintf_null_count0_no_iph); TC_RUN(scprintf_n_disabled);
    return tc_end();
}
