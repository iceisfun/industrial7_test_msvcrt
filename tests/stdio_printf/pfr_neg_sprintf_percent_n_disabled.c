/* TC
 * id:    pfr_neg_sprintf_percent_n_disabled
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf _snprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_printf_count_output, sprintf (%n disabled by default; invalid parameter handler, EINVAL, -1)
 * note:  output before %n may be partially written; only return/errno/iph/n are asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int have_iph;
TC_CASE(sprintf_n)
{
    char b[16]; volatile int n = 0x1234; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, "abc%n", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x1234);
}
TC_CASE(snprintf_n)
{
    char b[16]; volatile int n = 0x1234; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _snprintf(b, sizeof b, "abc%n", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x1234);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(sprintf_n); TC_RUN(snprintf_n);
    return tc_end();
}
