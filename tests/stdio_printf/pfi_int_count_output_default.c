/* TC
 * id:    pfi_int_count_output_default
 * area:  stdio_printf
 * funcs:
 * procs: _get_printf_count_output _set_printf_count_output
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _get_printf_count_output, _set_printf_count_output (default disabled since VS2005)
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
TC_DECL_PROC(int, _set_printf_count_output, (int));

TC_CASE(default_off) { if (!tc_p__get_printf_count_output) TC_SKIP("none"); CHECK_EQ_I(tc_p__get_printf_count_output(), 0); }
TC_CASE(set_get)
{
    if (!tc_p__set_printf_count_output || !tc_p__get_printf_count_output) TC_SKIP("none");
    CHECK_EQ_I(tc_p__set_printf_count_output(1), 0);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 1);
    CHECK_EQ_I(tc_p__set_printf_count_output(0), 1);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_get_printf_count_output); TC_LOAD_PROC(_set_printf_count_output);
  CHECK(tc_p__get_printf_count_output != NULL); CHECK(tc_p__set_printf_count_output != NULL);
  TC_RUN(default_off); TC_RUN(set_get); return tc_end(); }
