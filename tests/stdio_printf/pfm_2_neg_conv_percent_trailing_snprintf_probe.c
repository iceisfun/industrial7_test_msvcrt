/* TC
 * id:    pfm_2_neg_conv_percent_trailing_snprintf_probe
 * area:  stdio_printf
 * funcs: sprintf _snprintf
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf format specification (incomplete specification -> invalid parameter)
 * note:  PROBE: sprintf("abc%") and _snprintf(b, n, "x%") on ucrtbase return -1 with EINVAL and one invalid-parameter call (the format validation is
 *        shared with the _s variants). Buffer contents are not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(sprintf_abc) { char b[32]; int r; const char * volatile f = "abc%"; tc_iph_count = 0; errno = 0; r = sprintf(b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(snprintf_x)  { char b[32]; int r; const char * volatile f = "x%"; tc_iph_count = 0; errno = 0; r = _snprintf(b, sizeof b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_abc); TC_RUN(snprintf_x);
    return tc_end();
}
