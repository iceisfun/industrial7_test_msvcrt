/* TC
 * id:    pfi_neg_lone_percent_end
 * area:  stdio_printf
 * funcs: sprintf
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf format specification: incomplete format specification (sprintf, non-_s: no validation, observed)
 * note:  Observed on Win10 22H2 19045 ucrtbase: the non-_s printf family does not validate format strings (no handler call, no errno); the bad conversion char is copied through.
 *        A (test bug): sprintf "abc%" -> 3 ("abc"), "%-" -> 0, "%." -> 0.
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
TC_CASE(abc_pct)  { CHECK_EQ_I(try_fmt("abc%"), 3); CHECK_STR(g_b, "abc"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(pct_minus){ CHECK_EQ_I(try_fmt("%-"), 0);   CHECK_STR(g_b, ""); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
TC_CASE(pct_dot)  { CHECK_EQ_I(try_fmt("%."), 0);   CHECK_STR(g_b, ""); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(abc_pct); TC_RUN(pct_minus); TC_RUN(pct_dot);
    return tc_end();
}
