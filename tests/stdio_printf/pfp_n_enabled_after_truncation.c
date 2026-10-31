/* TC
 * id:    pfp_n_enabled_after_truncation
 * area:  stdio_printf
 * funcs: _snprintf
 * procs: _set_printf_count_output
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf, printf type n
 * note:  PROBE: on ucrtbase, with a truncated _snprintf the %n count still counts all characters produced (7 for "abcdef1"), not only the 3 stored.
 *        Pre-UCRT behaviour is probed separately (pfp_n_trunc_old_probe).
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));

TC_CASE(snprintf_count_full)
{
    char b[8]; int n = -1;
    _snprintf(b, 3, "abcdef%d%n", 1, &n);
    CHECK_EQ_I(n, 7);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__missing procs\n"); return 2; }
    tc_p__set_printf_count_output(1);
    TC_RUN(snprintf_count_full);
    tc_p__set_printf_count_output(0);
    return tc_end();
}
