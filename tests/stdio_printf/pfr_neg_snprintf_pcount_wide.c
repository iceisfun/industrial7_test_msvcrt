/* TC
 * id:    pfr_neg_snprintf_pcount_wide
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf
 * crt:   >=80
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _set_printf_count_output, printf_s %n ("%n is disabled by default since VS2005; invalid parameter")
 * note:  %n in a wide format is rejected: returns -1, errno EINVAL, handler runs, target int untouched.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(pcount_rejected)
{
    wchar_t b[16]; volatile int n = 0x1234; int r;
    int *volatile pn = (int *)&n;
    b[0] = L'Z';
    tc_iph_count = 0; errno = 0;
    r = _snwprintf(b, 16, L"x%n", pn);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
    CHECK_EQ_I(n, 0x1234);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(pcount_rejected);
    return tc_end();
}
