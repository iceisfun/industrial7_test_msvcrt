/* TC
 * id:    pfp_get_set_count_output
 * area:  stdio_printf
 * funcs:
 * procs: _set_printf_count_output, _get_printf_count_output
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _get_printf_count_output, _set_printf_count_output: set returns the previous setting; get returns the current one; default disabled
 */
#include "tc.h"

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _get_printf_count_output, (void));

TC_CASE(exported) { CHECK(tc_p__set_printf_count_output != NULL); CHECK(tc_p__get_printf_count_output != NULL); }
TC_CASE(sequence)
{
    int orig;
    if (!tc_p__set_printf_count_output || !tc_p__get_printf_count_output) TC_SKIP("not exported");
    orig = tc_p__get_printf_count_output();
    CHECK_EQ_I(orig, 0);
    CHECK_EQ_I(tc_p__set_printf_count_output(1), 0);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 1);
    CHECK_EQ_I(tc_p__set_printf_count_output(0), 1);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
    tc_p__set_printf_count_output(orig);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_get_printf_count_output);
    TC_RUN(exported); TC_RUN(sequence);
    return tc_end();
}
