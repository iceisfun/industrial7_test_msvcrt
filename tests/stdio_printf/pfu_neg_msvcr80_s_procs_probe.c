/* TC
 * id:    pfu_neg_msvcr80_s_procs_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s _snprintf_s swprintf_s _set_invalid_parameter_handler fprintf_s
 * crt:   msvcr80
 * tier:  p2
 * conf:  documented
 * ref:   MS docs sprintf_s, _snprintf_s, Parameter Validation (VS2005 CRT)
 * note:  mingw's libmsvcr80.a lacks the secure printf functions, so they are resolved with GetProcAddress. Missing exports are failures.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
TC_DECL_PROC(int, swprintf_s, (wchar_t *, size_t, const wchar_t *, ...));
TC_DECL_PROC(int, fprintf_s, (void *, const char *, ...));

TC_CASE(exports) { CHECK(tc_p_sprintf_s != NULL); CHECK(tc_p__snprintf_s != NULL); CHECK(tc_p_swprintf_s != NULL); CHECK(tc_p_fprintf_s != NULL); }
TC_CASE(basic)
{
    char b[16];
    if (!tc_p_sprintf_s) TC_SKIP("missing");
    CHECK_EQ_I(tc_p_sprintf_s(b, 16, "ab%d", 12), 4); CHECK_STR(b, "ab12");
}
TC_CASE(small_buffer)
{
    char b[16]; int r;
    if (!tc_p_sprintf_s) TC_SKIP("missing");
    b[0] = 'Q'; tc_iph_count = 0;
    r = tc_p_sprintf_s(b, 2, "abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(truncate)
{
    char b[16]; int r;
    if (!tc_p__snprintf_s) TC_SKIP("missing");
    tc_iph_count = 0;
    r = tc_p__snprintf_s(b, 4, (size_t)-1, "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_format)
{
    char b[16]; int r;
    if (!tc_p_sprintf_s) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0;
    r = tc_p_sprintf_s(b, 16, NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(sprintf_s); TC_LOAD_PROC(_snprintf_s); TC_LOAD_PROC(swprintf_s); TC_LOAD_PROC(fprintf_s);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(exports); TC_RUN(basic); TC_RUN(small_buffer); TC_RUN(truncate); TC_RUN(null_format);
    return tc_end();
}
