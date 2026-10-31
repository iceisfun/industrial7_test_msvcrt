/* TC
 * id:    pfo_neg_probe_default_handler_exitcode
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _invalid_parameter; UCRT invalid_parameter_noinfo_noreturn -> __fastfail(FAST_FAIL_INVALID_ARG)
 * note:  PROBE: the exit code of a child that hits the default invalid-parameter handler via __stdio_common_vsprintf_s(NULL buffer)
 *        is exactly STATUS_STACK_BUFFER_OVERRUN (0xC0000409). Other plausible values on real Windows: 0xC000041D, 3 (abort).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *vsprintf_s_t)(unsigned long long, char *, size_t, const char *, _locale_t, va_list);
static vsprintf_s_t p_vsprintf_s;
static int vss(char *b, size_t n, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = p_vsprintf_s(0, b, n, fmt, NULL, ap);
    va_end(ap);
    return r;
}
TC_CASE(exit_code)
{
    DWORD c = 0; CHECK(tc_spawn_self("fatal", &c, 20000));
    CHECK_EQ_U(c, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    p_vsprintf_s = (vsprintf_s_t)tc_crt_proc("__stdio_common_vsprintf_s");
    if (!p_vsprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    if (tc_child_name()) { char *volatile nb = NULL; vss(nb, 16, "abc"); return 77; }
    TC_RUN(exit_code);
    return tc_end();
}
