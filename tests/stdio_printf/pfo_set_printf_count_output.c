/* TC
 * id:    pfo_set_printf_count_output
 * area:  stdio_printf
 * funcs:
 * procs: _set_printf_count_output _get_printf_count_output
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_printf_count_output ("returns the previous setting"), _get_printf_count_output ("%n is disabled by default", returns 0 or 1... nonzero = enabled)
 * note:  msvcr80 is excluded (import-library gap); msvcrt.dll does not export these. Non-zero values other than 1 are in pfo_set_printf_count_output_nonzero_probe.
 */
#include "tc.h"

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _get_printf_count_output, (void));

TC_CASE(exported) { CHECK(tc_p__set_printf_count_output != NULL); CHECK(tc_p__get_printf_count_output != NULL); }
TC_CASE(sequence)
{
    if (!tc_p__set_printf_count_output || !tc_p__get_printf_count_output) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
    CHECK_EQ_I(tc_p__set_printf_count_output(1), 0);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 1);
    CHECK_EQ_I(tc_p__set_printf_count_output(1), 1);
    CHECK_EQ_I(tc_p__set_printf_count_output(0), 1);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
    CHECK_EQ_I(tc_p__set_printf_count_output(0), 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_get_printf_count_output);
    TC_RUN(exported); TC_RUN(sequence);
    if (tc_p__set_printf_count_output) tc_p__set_printf_count_output(0);
    return tc_end();
}
