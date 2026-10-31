/* TC
 * id:    pfi_neg_unknown_conversion
 * area:  stdio_printf
 * funcs: sprintf
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs printf format specification: invalid format string -> invalid parameter handler, -1, EINVAL
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
TC_CASE(pct_y)       { CHECK_EQ_I(try_fmt("%y"), -1);   CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(truncated)   { CHECK_EQ_I(try_fmt("%5"), -1);   CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(bad_flag)    { CHECK_EQ_I(try_fmt("%!d"), -1);  CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(after_valid) { CHECK_EQ_I(try_fmt("%d %y"), -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(pct_y); TC_RUN(truncated); TC_RUN(bad_flag); TC_RUN(after_valid);
    return tc_end();
}
