/* TC
 * id:    pfo_set_printf_count_output_nonzero_probe
 * area:  stdio_printf
 * funcs:
 * procs: _set_printf_count_output _get_printf_count_output
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_printf_count_output ("non-zero enables")
 * note:  PROBES that after _set_printf_count_output(42) _get_printf_count_output returns exactly 1 (value normalised),
 *        and that a later set(0) returns previous 1.
 */
#include "tc.h"

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _get_printf_count_output, (void));

TC_CASE(normalised)
{
    if (!tc_p__set_printf_count_output || !tc_p__get_printf_count_output) TC_SKIP("not exported");
    tc_p__set_printf_count_output(0);
    CHECK_EQ_I(tc_p__set_printf_count_output(42), 0);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 1);
    CHECK_EQ_I(tc_p__set_printf_count_output(0), 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_get_printf_count_output);
    TC_RUN(normalised);
    if (tc_p__set_printf_count_output) tc_p__set_printf_count_output(0);
    return tc_end();
}
