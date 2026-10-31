/* TC
 * id:    pfp_neg_n_null_ptr
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * procs: _set_printf_count_output, _get_printf_count_output, _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf type n (pointer to integer)
 * note:  Restricted to ucrtbase: pre-UCRT CRTs are not known to validate the %n pointer and a NULL store would crash the process.
 *        PROBE: with %n enabled a NULL pointer argument is rejected as an invalid parameter (-1, EINVAL, handler called) instead of crashing.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _get_printf_count_output, (void));

TC_CASE(n_null)
{
    char b[16]; int *volatile p = NULL; int r; tc_iph_count = 0; errno = 0;
    r = sprintf(b, "x%n", p);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(hn_null)
{
    char b[16]; short *volatile p = NULL; int r; tc_iph_count = 0; errno = 0;
    r = sprintf(b, "x%hn", p);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(restored)
{
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_get_printf_count_output);
    if (!tc_p__set_printf_count_output || !tc_p__get_printf_count_output || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__missing procs\n"); return 2; }
    tc_p__set_printf_count_output(1);
    TC_RUN(n_null); TC_RUN(hn_null);
    tc_p__set_printf_count_output(0);
    TC_RUN(restored);
    return tc_end();
}
