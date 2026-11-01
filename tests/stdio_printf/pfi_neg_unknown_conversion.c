/* TC
 * id:    pfi_neg_unknown_conversion
 * area:  stdio_printf
 * funcs: sprintf
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf format specification: invalid format string (sprintf, non-_s: copied through, no handler; observed)
 * note:  Observed on Win10 22H2 19045 ucrtbase: the non-_s printf family does not validate format strings (no handler call, no errno); the bad conversion char is copied through.
 *        A (test bug): "%y"->"y"(1), "%5"->""(0), "%!d"->"!d"(2), "%d %y"->"1 y"(3).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
static char g_b[32];
static int try_fmt(const char *volatile f)
{
    char *b = g_b; int r;
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, f, 1);
    return r;
}
TC_CASE(pct_y)       { CHECK_EQ_I(try_fmt("%y"), 1);   CHECK_STR(g_b, "y"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(truncated)   { CHECK_EQ_I(try_fmt("%5"), 0);   CHECK_STR(g_b, ""); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(bad_flag)    { CHECK_EQ_I(try_fmt("%!d"), 2);  CHECK_STR(g_b, "!d"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(after_valid) { CHECK_EQ_I(try_fmt("%d %y"), 3); CHECK_STR(g_b, "1 y"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(pct_y); TC_RUN(truncated); TC_RUN(bad_flag); TC_RUN(after_valid);
    return tc_end();
}
