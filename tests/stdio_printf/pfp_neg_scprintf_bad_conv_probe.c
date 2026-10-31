/* TC
 * id:    pfp_neg_scprintf_bad_conv_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _scprintf; "Parameter validation"
 * note:  PROBE: _scprintf with an invalid conversion ('%y') and with a trailing lone '%' ("abc%") is expected to
 *        return -1, call the invalid-parameter handler once and set errno to EINVAL on ucrtbase.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(probe_y)
{
    int r; tc_iph_count = 0; errno = 0;
    r = _scprintf("%y");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(probe_trailing)
{
    int r; tc_iph_count = 0; errno = 0;
    r = _scprintf("abc%");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(probe_errno)
{
    int r; errno = 0; tc_iph_count = 0;
    r = _scprintf("%y");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(valid_control)
{
    int r; volatile int v = 12345; tc_iph_count = 0;
    r = _scprintf("n=%d", v);
    CHECK_EQ_I(r, 7); CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(probe_y); TC_RUN(probe_trailing); TC_RUN(probe_errno); TC_RUN(valid_control);
    return tc_end();
}
