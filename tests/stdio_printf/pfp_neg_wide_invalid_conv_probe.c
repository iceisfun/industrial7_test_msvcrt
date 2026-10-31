/* TC
 * id:    pfp_neg_wide_invalid_conv_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf
 * procs: _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "Parameter validation"
 * note:  PROBE: (a) swprintf with a format ending in a lone '%' returns <0 and calls the handler once;
 *        (b) errno is EINVAL after swprintf with unknown conversion '%y'.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(trailing_pct)
{
    wchar_t w[16]; const wchar_t *volatile f = L"abc%"; int r;
    tc_iph_count = 0;
    r = swprintf(w, 16, f);
    CHECK(r < 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(unknown_conv_errno)
{
    wchar_t w[16]; const wchar_t *volatile f = L"%y"; int r;
    tc_iph_count = 0; errno = 0;
    r = swprintf(w, 16, f);
    CHECK(r < 0);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(trailing_pct); TC_RUN(unknown_conv_errno);
    return tc_end();
}
