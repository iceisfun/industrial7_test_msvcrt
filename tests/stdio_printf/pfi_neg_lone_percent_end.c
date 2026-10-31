/* TC
 * id:    pfi_neg_lone_percent_end
 * area:  stdio_printf
 * funcs: sprintf
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs printf format specification: incomplete format specification -> invalid parameter handler
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
static int try_fmt(const char *volatile f)
{
    char b[32]; int r;
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, f, 1);
    return r;
}
TC_CASE(abc_pct)  { CHECK_EQ_I(try_fmt("abc%"), -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(pct_minus){ CHECK_EQ_I(try_fmt("%-"), -1);   CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(pct_dot)  { CHECK_EQ_I(try_fmt("%."), -1);   CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(abc_pct); TC_RUN(pct_minus); TC_RUN(pct_dot);
    return tc_end();
}
