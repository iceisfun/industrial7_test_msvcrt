/* TC
 * id:    pff_printf_count_output_enabled
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _get_printf_count_output _set_printf_count_output
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_printf_count_output (%n disabled by default since VS2005 SP1)
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
TC_DECL_PROC(int, _set_printf_count_output, (int));

TC_CASE(enabled_n)
{
    char b[32]; volatile double v = 1.5; int n = -1, r, prev;
    if (!tc_p__set_printf_count_output) TC_SKIP("none");
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
    prev = tc_p__set_printf_count_output(1); CHECK_EQ_I(prev, 0);
    r = sprintf(b, "%.2f%n", v, &n);
    CHECK_STR(b, "1.50"); CHECK_EQ_I(n, 4); CHECK_EQ_I(r, 4);
    prev = tc_p__set_printf_count_output(0); CHECK_EQ_I(prev, 1);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
}
TC_CASE(n_after_wide_conv)
{
    char b[64]; volatile double v = 12.5; int n = -1;
    if (!tc_p__set_printf_count_output) TC_SKIP("none");
    tc_p__set_printf_count_output(1);
    sprintf(b, "%8.3f|%n", v, &n);
    tc_p__set_printf_count_output(0);
    CHECK_STR(b, "  12.500|"); CHECK_EQ_I(n, 9);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_get_printf_count_output); TC_LOAD_PROC(_set_printf_count_output);
  CHECK(tc_p__set_printf_count_output != NULL); CHECK(tc_p__get_printf_count_output != NULL);
  TC_RUN(enabled_n); TC_RUN(n_after_wide_conv); return tc_end(); }
