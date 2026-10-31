/* TC
 * id:    pfi_neg_probe_swprintf_s_incomplete_spec
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   none (incomplete conversion specification at end of format)
 * note:  PROBE: whether a format ending in an incomplete spec ('%5', 'd%') is rejected with -1/EINVAL/invalid-parameter in swprintf_s. Expected: yes (ucrt treats it as an invalid format).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static const wchar_t *volatile f_w = L"%5";
static const wchar_t *volatile f_p = L"%d%";

TC_CASE(width_only)
{
    wchar_t b[32]; int n; errno = 0; tc_iph_count = 0;
    n = swprintf_s(b, 32, f_w, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(trailing_percent)
{
    wchar_t b[32]; int n; errno = 0; tc_iph_count = 0;
    n = swprintf_s(b, 32, f_p, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(width_only); TC_RUN(trailing_percent);
    return tc_end();
}
