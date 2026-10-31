/* TC
 * id:    pfu_neg_pct_n_sprintf_s
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * procs: _get_printf_count_output
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf format specification (%n), _set_printf_count_output: %n is disabled by default; using it invokes the invalid parameter handler
 * note:  Cases are skipped if the process-wide %n setting was already non-zero (it is 0 by default). Output buffer contents after the failure are not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
static int def_zero;

TC_CASE(default_zero)
{
    CHECK(tc_p__get_printf_count_output != NULL);
    def_zero = tc_p__get_printf_count_output ? tc_p__get_printf_count_output() == 0 : 0;
    CHECK(def_zero);
}
TC_CASE(rejected_untouched)
{
    char b[32]; int n = 12345, r; volatile const char *f = "ab%n";
    if (!def_zero) TC_SKIP("%n enabled");
    tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, 32, (const char *)f, &n);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(n, 12345);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(only_pct_n)
{
    char b[32]; int n = -7, r; volatile const char *f = "%n";
    if (!def_zero) TC_SKIP("%n enabled");
    tc_iph_count = 0;
    r = sprintf_s(b, 32, (const char *)f, &n);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(n, -7);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(after_conversions)
{
    char b[32]; int n = 99, r; volatile const char *f = "%d:%s%n";
    if (!def_zero) TC_SKIP("%n enabled");
    tc_iph_count = 0;
    r = sprintf_s(b, 32, (const char *)f, 5, "xy", &n);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(n, 99);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_LOAD_PROC(_get_printf_count_output);
    TC_RUN(default_zero); TC_RUN(rejected_untouched); TC_RUN(only_pct_n); TC_RUN(after_conversions);
    return tc_end();
}
