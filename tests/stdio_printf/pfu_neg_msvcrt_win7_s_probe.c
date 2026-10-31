/* TC
 * id:    pfu_neg_msvcrt_win7_s_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s _snprintf_s swprintf_s _set_invalid_parameter_handler
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs sprintf_s, _snprintf_s
 * note:  Probing: the system msvcrt.dll (Vista+/Win7) exports the secure printf functions; on failure they return -1 with errno set (ERANGE for small buffer, EINVAL for NULL format). Whether the handler is called is recorded (not asserted). If the handler cannot be installed the failing-buffer cases are skipped, since the default may be fatal.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
TC_DECL_PROC(int, swprintf_s, (wchar_t *, size_t, const wchar_t *, ...));
static int have_iph;

TC_CASE(small_buffer)
{
    char b[16]; int r;
    if (!tc_p_sprintf_s) TC_SKIP("sprintf_s missing");
    if (!have_iph) TC_SKIP("no handler API");
    r = tc_p_sprintf_s(b, 2, "abc");
    CHECK_EQ_I(r, -1);
}
TC_CASE(truncate)
{
    char b[16]; int r;
    if (!tc_p__snprintf_s) TC_SKIP("_snprintf_s missing");
    r = tc_p__snprintf_s(b, 4, (size_t)-1, "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc");
}
TC_CASE(null_format)
{
    char b[16]; int r;
    if (!tc_p_sprintf_s) TC_SKIP("sprintf_s missing");
    if (!have_iph) TC_SKIP("no handler API");
    errno = 0;
    r = tc_p_sprintf_s(b, 16, NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(wide_small)
{
    wchar_t w[8]; int r;
    if (!tc_p_swprintf_s) TC_SKIP("swprintf_s missing");
    if (!have_iph) TC_SKIP("no handler API");
    r = tc_p_swprintf_s(w, 1, L"ab");
    CHECK_EQ_I(r, -1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(sprintf_s); TC_LOAD_PROC(_snprintf_s); TC_LOAD_PROC(swprintf_s);
    have_iph = tc_iph_install();
    TC_RUN(small_buffer); TC_RUN(truncate); TC_RUN(null_format); TC_RUN(wide_small);
    return tc_end();
}
