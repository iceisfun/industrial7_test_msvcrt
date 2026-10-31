/* TC
 * id:    pfu_pct_n_enabled_probe
 * area:  stdio_printf
 * funcs: sprintf_s
 * procs: _set_printf_count_output _get_printf_count_output
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_printf_count_output, _get_printf_count_output
 * note:  PROBING: whether the secure sprintf_s honours _set_printf_count_output(1), i.e. %n then stores the count (return 3, n==3) instead of invoking the invalid parameter handler. The docs say the setting applies to the printf family; sprintf_s is assumed to be included. Not run on ucrtbase (see pfu_pct_n_enabled_probe_ucrt). The setting is restored to 0 at the end of every case.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _get_printf_count_output, (void));

TC_CASE(enable_returns_old)
{
    int old;
    CHECK(tc_p__set_printf_count_output && tc_p__get_printf_count_output);
    if (!tc_p__set_printf_count_output) return;
    old = tc_p__set_printf_count_output(1);
    CHECK_EQ_I(old, 0);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 1);
    tc_p__set_printf_count_output(0);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
}
TC_CASE(sprintf_s_stores_count)
{
    char b[32]; int n = -1, r; volatile const char *f = "abc%n";
    if (!tc_p__set_printf_count_output) return;
    tc_p__set_printf_count_output(1);
    tc_iph_count = 0;
    r = sprintf_s(b, 32, (const char *)f, &n);
    tc_p__set_printf_count_output(0);
    CHECK_EQ_I(r, 3);
    CHECK_EQ_I(n, 3);
    CHECK_STR(b, "abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(rejected_again_after_restore)
{
    char b[32]; int n = 77, r; volatile const char *f = "abc%n";
    if (!tc_p__set_printf_count_output) return;
    tc_p__set_printf_count_output(1);
    tc_p__set_printf_count_output(0);
    tc_iph_count = 0;
    r = sprintf_s(b, 32, (const char *)f, &n);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(n, 77);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_get_printf_count_output);
    TC_RUN(enable_returns_old); TC_RUN(sprintf_s_stores_count); TC_RUN(rejected_again_after_restore);
    return tc_end();
}
