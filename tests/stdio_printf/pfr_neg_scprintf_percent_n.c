/* TC
 * id:    pfr_neg_scprintf_percent_n
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf
 * crt:   >=80
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _set_printf_count_output (%n disabled by default since VS2005), _scprintf
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int have_iph;
TC_CASE(scprintf_percent_n)
{
    volatile int n = 0x1234; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _scprintf("ab%n", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x1234);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(scprintf_percent_n);
    return tc_end();
}
