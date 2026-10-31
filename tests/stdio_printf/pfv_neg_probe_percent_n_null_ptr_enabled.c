/* TC
 * id:    pfv_neg_probe_percent_n_null_ptr_enabled
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * procs: _set_printf_count_output
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_printf_count_output; printf %n
 * note:  PROBING: with %n enabled, sprintf(b,"ab%n",(int*)NULL) is expected to be an invalid parameter (-1, EINVAL, handler once)
 *        and not an access violation. Runs in a child process so a crash on some generation cannot affect the parent.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));

static int child_main(const char *name)
{
    char b[16]; int *volatile p = NULL; int n;
    (void)name;
    tc_iph_install();
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) return 20;
    tc_p__set_printf_count_output(1);
    tc_iph_count = 0; errno = 0;
    n = sprintf(b, "ab%n", p);
    if (n == -1 && tc_iph_count == 1 && errno == EINVAL) return 10;
    if (n == -1 && tc_iph_count == 1) return 11;
    if (n == -1) return 12;
    return 13;
}
TC_CASE(null_ptr_rejected) { DWORD code = 0; CHECK(tc_spawn_self("nullptr", &code, 20000)); CHECK_EQ_U(code, 10); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(null_ptr_rejected);
    return tc_end();
}
