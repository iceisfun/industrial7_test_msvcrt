/* TC
 * id:    pfp_neg_sprintf_s_errno_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sprintf_s; "Parameter validation"
 * note:  PROBE: errno value after each failing sprintf_s call. Expected ERANGE for too-small buffer,
 *        EINVAL for size 0 and for NULL format. Each is its own case so one wrong guess masks nothing.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(too_small_erange)
{
    char b[16]; volatile int v = 12345; int r;
    errno = 0; tc_iph_count = 0;
    r = sprintf_s(b, 3, "%d", v);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(zero_size_einval)
{
    char b[16]; volatile size_t sz = 0; int r;
    errno = 0; tc_iph_count = 0;
    r = sprintf_s(b, sz, "%d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format_einval)
{
    char b[16]; const char *volatile f = NULL; int r;
    errno = 0; tc_iph_count = 0;
    r = sprintf_s(b, sizeof b, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(too_small_erange); TC_RUN(zero_size_einval); TC_RUN(null_format_einval);
    return tc_end();
}
