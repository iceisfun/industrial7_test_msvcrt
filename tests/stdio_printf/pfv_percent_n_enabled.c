/* TC
 * id:    pfv_percent_n_enabled
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_printf_count_output _get_printf_count_output
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_printf_count_output
 * note:  get==0 by default; set(1) returns previous (0); %n then stores count; set(0) restores. Disabled state: pfv_neg_percent_n_disabled.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
TC_DECL_PROC(int, _set_printf_count_output, (int));

TC_CASE(default_disabled_flag) { CHECK_EQ_I(tc_p__get_printf_count_output(), 0); }
TC_CASE(enable_returns_prev) { CHECK_EQ_I(tc_p__set_printf_count_output(1), 0); CHECK_EQ_I(tc_p__get_printf_count_output(), 1); }
TC_CASE(n_stores_count)
{
    char b[64]; volatile int n = -1; int r;
    tc_p__set_printf_count_output(1);
    r = sprintf(b, "abc%n", (int *)&n);
    CHECK_STR(b, "abc"); CHECK_EQ_I(r, 3); CHECK_EQ_I(n, 3);
    r = sprintf(b, "%d%n!", 12345, (int *)&n);
    CHECK_STR(b, "12345!"); CHECK_EQ_I(r, 6); CHECK_EQ_I(n, 5);
}
TC_CASE(restore) { CHECK_EQ_I(tc_p__set_printf_count_output(0), 1); CHECK_EQ_I(tc_p__get_printf_count_output(), 0); }

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_printf_count_output); TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__get_printf_count_output || !tc_p__set_printf_count_output) { CHECK(0 && "exports missing"); return tc_end() + 1; }
    TC_RUN(default_disabled_flag); TC_RUN(enable_returns_prev); TC_RUN(n_stores_count); TC_RUN(restore);
    return tc_end();
}
