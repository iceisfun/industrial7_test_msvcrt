/* TC
 * id:    pfr_set_printf_count_output
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_printf_count_output _get_printf_count_output
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_printf_count_output, _get_printf_count_output (%n disabled by default since VS2005; returns previous setting)
 * note:  Flag restored to 0 at the end.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _get_printf_count_output, (void));

TC_CASE(default_disabled)
{
    CHECK(tc_p__get_printf_count_output != NULL);
    if (!tc_p__get_printf_count_output) return;
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
}
TC_CASE(round_trip)
{
    if (!tc_p__set_printf_count_output || !tc_p__get_printf_count_output) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__set_printf_count_output(1), 0);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 1);
    CHECK_EQ_I(tc_p__set_printf_count_output(0), 1);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
}
TC_CASE(enabled_n_works)
{
    char b[32]; int n = -1, r;
    if (!tc_p__set_printf_count_output) TC_SKIP("not exported");
    tc_p__set_printf_count_output(1);
    r = sprintf(b, "abc%n", &n);
    tc_p__set_printf_count_output(0);
    CHECK_EQ_I(r, 3);
    CHECK_EQ_I(n, 3);
    CHECK_STR(b, "abc");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_get_printf_count_output);
    TC_RUN(default_disabled); TC_RUN(round_trip); TC_RUN(enabled_n_works);
    if (tc_p__set_printf_count_output) tc_p__set_printf_count_output(0);
    return tc_end();
}
