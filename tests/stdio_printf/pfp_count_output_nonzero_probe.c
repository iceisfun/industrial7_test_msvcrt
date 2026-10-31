/* TC
 * id:    pfp_count_output_nonzero_probe
 * area:  stdio_printf
 * funcs:
 * procs: _set_printf_count_output, _get_printf_count_output
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _set_printf_count_output (documents only enable/disable)
 * note:  PROBE: any nonzero value is normalized to 1 when stored (get returns 1, not 5 / -1), and a following set returns the normalized previous value.
 */
#include "tc.h"

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _get_printf_count_output, (void));

TC_CASE(five_normalized)
{
    int orig;
    if (!tc_p__set_printf_count_output || !tc_p__get_printf_count_output) TC_SKIP("not exported");
    orig = tc_p__get_printf_count_output();
    tc_p__set_printf_count_output(5);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 1);
    tc_p__set_printf_count_output(orig ? 1 : 0);
}
TC_CASE(minus_one_normalized)
{
    int orig;
    if (!tc_p__set_printf_count_output || !tc_p__get_printf_count_output) TC_SKIP("not exported");
    orig = tc_p__get_printf_count_output();
    tc_p__set_printf_count_output(-1);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 1);
    tc_p__set_printf_count_output(orig ? 1 : 0);
}
TC_CASE(previous_of_two)
{
    int orig;
    if (!tc_p__set_printf_count_output || !tc_p__get_printf_count_output) TC_SKIP("not exported");
    orig = tc_p__get_printf_count_output();
    tc_p__set_printf_count_output(7);
    CHECK_EQ_I(tc_p__set_printf_count_output(2), 1);
    tc_p__set_printf_count_output(orig ? 1 : 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_get_printf_count_output);
    TC_RUN(five_normalized); TC_RUN(minus_one_normalized); TC_RUN(previous_of_two);
    return tc_end();
}
