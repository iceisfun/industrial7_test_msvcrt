/* TC
 * id:    pff_neg_scprintf_invalid_after_float_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs Parameter validation; printf format specification (unknown conversion characters are invalid)
 * note:  PROBES: _scprintf("%f %y", 1.0, 2) - a valid float conversion followed by an unknown conversion 'y' - returns -1,
 *        calls the invalid-parameter handler exactly once, and sets errno EINVAL (the count is not "8 + 1").
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(unknown_conv_after_float)
{
    const char * volatile f = "%f %y"; int r; tc_iph_count = 0; errno = 0;
    r = _scprintf(f, 1.0, 2);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(unknown_conv_before_float)
{
    const char * volatile f = "%y %f"; int r; tc_iph_count = 0; errno = 0;
    r = _scprintf(f, 2, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(unknown_conv_after_float); TC_RUN(unknown_conv_before_float);
    return tc_end();
}
