/* TC
 * id:    pfp_n_trunc_old_probe
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf, _set_printf_count_output
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf
 * note:  PROBE: pre-UCRT output engines stop formatting once the bounded string stream overflows, so a %n that follows the truncation point
 *        is never reached and its target is left untouched (the -1 sentinel). Return value is not asserted here.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _snprintf, (char *, size_t, const char *, ...));

TC_CASE(n_after_overflow_untouched)
{
    char b[8]; int n = -1;
    tc_p__snprintf(b, 3, "abcdef%d%n", 1, &n);
    CHECK_EQ_I(n, -1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_snprintf);
    if (!tc_p__set_printf_count_output || !tc_p__snprintf) { tc_write("TC FAIL " TC_TEST_ID "::__missing procs\n"); return 2; }
    tc_p__set_printf_count_output(1);
    TC_RUN(n_after_overflow_untouched);
    tc_p__set_printf_count_output(0);
    return tc_end();
}
