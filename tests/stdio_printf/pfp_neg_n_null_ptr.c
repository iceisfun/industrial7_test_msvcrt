/* TC
 * id:    pfp_neg_n_null_ptr
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * procs: _set_printf_count_output, _get_printf_count_output, _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * conf:  observed
 * ref:   MS docs printf type n (pointer to integer)
 * note:  Observed on Windows 10 22H2 ucrtbase: with %n enabled (_set_printf_count_output(1)) a NULL pointer argument is NOT validated by the non-secure
 *        sprintf: the store faults and the process dies with STATUS_ACCESS_VIOLATION (0xC0000005), even with an invalid-parameter handler installed.
 *        Runs in a child process; the parent only asserts the exit status. (Class C: the old EINVAL / handler expectation never held.)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _get_printf_count_output, (void));

TC_CASE(n_null_access_violation)
{
    DWORD code = 0; int ran = tc_spawn_self("n", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(hn_null_access_violation)
{
    DWORD code = 0; int ran = tc_spawn_self("hn", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(restored)
{
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_get_printf_count_output);
    if (!tc_p__set_printf_count_output || !tc_p__get_printf_count_output || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__missing procs\n"); return 2; }
    if ((c = tc_child_name()) != NULL) {
        char b[16]; int *volatile p = NULL; short *volatile hp = NULL;
        tc_p__set_printf_count_output(1);
        if (c[0] == 'h') sprintf(b, "x%hn", hp); else sprintf(b, "x%n", p);
        return 77;   /* not reached: the unvalidated store through NULL faults */
    }
    TC_RUN(n_null_access_violation); TC_RUN(hn_null_access_violation);
    TC_RUN(restored);
    return tc_end();
}
