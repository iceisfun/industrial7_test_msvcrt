/* TC
 * id:    pfp_n_enabled_basic
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_printf_count_output
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf type field n; _set_printf_count_output
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));

TC_CASE(simple)
{
    char b[64]; int n = -1, r;
    r = sprintf(b, "abc%n", &n);
    CHECK_STR(b, "abc"); CHECK_EQ_I(n, 3); CHECK_EQ_I(r, 3);
}
TC_CASE(after_int)
{
    char b[64]; int n = -1, r;
    r = sprintf(b, "%d%n", 12345, &n);
    CHECK_STR(b, "12345"); CHECK_EQ_I(n, 5); CHECK_EQ_I(r, 5);
}
TC_CASE(two_positions)
{
    char b[64]; int n1 = -1, n2 = -1, r;
    r = sprintf(b, "ab%ncde%nf", &n1, &n2);
    CHECK_STR(b, "abcdef"); CHECK_EQ_I(n1, 2); CHECK_EQ_I(n2, 5); CHECK_EQ_I(r, 6);
}
TC_CASE(with_width_and_percent)
{
    char b[64]; int n = -1, m = -1;
    sprintf(b, "%5d%n", 42, &n);
    CHECK_STR(b, "   42"); CHECK_EQ_I(n, 5);
    sprintf(b, "100%%%n", &m);
    CHECK_STR(b, "100%"); CHECK_EQ_I(m, 4);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_printf_count_output\n"); return 2; }
    tc_p__set_printf_count_output(1);
    TC_RUN(simple); TC_RUN(after_int); TC_RUN(two_positions); TC_RUN(with_width_and_percent);
    tc_p__set_printf_count_output(0);
    return tc_end();
}
