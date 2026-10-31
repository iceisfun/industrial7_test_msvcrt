/* TC
 * id:    pfv_neg_percent_n_null_pointer_enabled
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * procs: _set_printf_count_output _get_printf_count_output
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_printf_count_output; printf: %n rejected again once the count output is disabled
 * note:  The NULL-pointer-with-%n-enabled assertion lives in pfv_neg_probe_percent_n_null_ptr_enabled (probe).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
TC_DECL_PROC(int, _set_printf_count_output, (int));

TC_CASE(set_returns_previous) { CHECK_EQ_I(tc_p__set_printf_count_output(1), 0); CHECK_EQ_I(tc_p__get_printf_count_output(), 1); }
TC_CASE(restore_and_rejected)
{
    char b[16]; volatile int t = 0x5a5a; int n;
    CHECK_EQ_I(tc_p__set_printf_count_output(0), 1); CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
    tc_iph_count = 0; n = sprintf(b, "ab%n", (int *)&t);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(t, 0x5a5a);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_LOAD_PROC(_get_printf_count_output); TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__get_printf_count_output || !tc_p__set_printf_count_output) { CHECK(0 && "exports missing"); return tc_end() + 1; }
    TC_RUN(set_returns_previous); TC_RUN(restore_and_rejected);
    return tc_end();
}
