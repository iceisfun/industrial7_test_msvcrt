/* TC
 * id:    pfi_neg_percent_n_disabled
 * area:  stdio_printf
 * funcs: sprintf
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_printf_count_output / printf type field n: %n disabled by default -> invalid parameter handler
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
TC_CASE(percent_n) {
    char b[32]; int n = 0x5a5a5a5a, r;
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, "ab%n", &n);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(n, 0x5a5a5a5a);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(percent_n);
    return tc_end();
}
