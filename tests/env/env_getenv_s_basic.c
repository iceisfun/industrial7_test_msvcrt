/* TC
 * id:    env_getenv_s_basic
 * area:  env
 * procs: getenv_s
 * funcs: _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs getenv_s, _wgetenv_s (pReturnValue is the size including the NUL; not found -> 0 and *pReturnValue = 0)
 * note:  getenv_s resolved at run time because some mingw import libraries lack it.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(int, getenv_s, (size_t *, char *, size_t, const char *));

TC_CASE(found_returns_0_and_len_plus_1)
{
    size_t n = 99; char b[32]; int r;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    _putenv("TCV_GS=hello");
    r = tc_p_getenv_s(&n, b, sizeof b, "TCV_GS");
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 6); CHECK_STR(b, "hello");
    _putenv("TCV_GS=");
}
TC_CASE(exact_fit_buffer_size_len_plus_1)
{
    size_t n = 99; char b[6]; int r;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    _putenv("TCV_GS=hello");
    r = tc_p_getenv_s(&n, b, 6, "TCV_GS");
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 6); CHECK_STR(b, "hello");
    _putenv("TCV_GS=");
}
TC_CASE(not_found_returns_0_len_0)
{
    size_t n = 99; char b[8]; int r;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    r = tc_p_getenv_s(&n, b, sizeof b, "TCV_GS_SURELY_MISSING_5");
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 0);
}
TC_CASE(case_insensitive_name)
{
    size_t n = 99; char b[16]; int r;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    _putenv("TCV_GS_CI=Val");
    r = tc_p_getenv_s(&n, b, sizeof b, "tcv_gs_ci");
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 4); CHECK_STR(b, "Val");
    _putenv("TCV_GS_CI=");
}
TC_CASE(size_query_null_buffer_zero_size_returns_needed_size)
{
    size_t n = 99; int r;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    _putenv("TCV_GS_Q=twelve chars");
    r = tc_p_getenv_s(&n, NULL, 0, "TCV_GS_Q");
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 13);
    _putenv("TCV_GS_Q=");
}
TC_CASE(two_byte_buffer_one_char_value)
{
    size_t n = 99; char b[2] = { 'x', 'x' }; int r;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    _putenv("TCV_GS_1=a");
    r = tc_p_getenv_s(&n, b, 2, "TCV_GS_1");
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 2); CHECK(b[0] == 'a' && b[1] == 0);
    _putenv("TCV_GS_1=");
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(getenv_s);
    TC_RUN(found_returns_0_and_len_plus_1); TC_RUN(exact_fit_buffer_size_len_plus_1); TC_RUN(not_found_returns_0_len_0);
    TC_RUN(case_insensitive_name); TC_RUN(size_query_null_buffer_zero_size_returns_needed_size); TC_RUN(two_byte_buffer_one_char_value);
    return tc_end();
}
