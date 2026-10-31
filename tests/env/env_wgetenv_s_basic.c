/* TC
 * id:    env_wgetenv_s_basic
 * area:  env
 * funcs: _wgetenv_s _wputenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0

 * conf:  documented
 * ref:   MS docs getenv_s, _wgetenv_s
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(found_len_plus_1_in_elements)
{
    size_t ret = 99; wchar_t buf[16]; errno_t e;
    _wputenv(L"TC_W1=hello");
    e = _wgetenv_s(&ret, buf, 16, L"TC_W1");
    CHECK_EQ_I(e, 0); CHECK_EQ_U(ret, 6); CHECK_WSTR(buf, L"hello");
}
TC_CASE(not_found_zero_and_empty_buf)
{
    size_t ret = 99; wchar_t buf[8] = L"xyz"; errno_t e;
    e = _wgetenv_s(&ret, buf, 8, L"TC_W_NOSUCH_VAR");
    CHECK_EQ_I(e, 0); CHECK_EQ_U(ret, 0); CHECK_EQ_U(buf[0], 0);
}
TC_CASE(size_query_null_buffer)
{
    size_t ret = 99; errno_t e;
    _wputenv(L"TC_W2=abcdefgh");
    e = _wgetenv_s(&ret, NULL, 0, L"TC_W2");
    CHECK_EQ_I(e, 0); CHECK_EQ_U(ret, 9);
}
TC_CASE(case_insensitive)
{
    size_t ret = 99; wchar_t buf[16]; errno_t e;
    _wputenv(L"TC_WCase=Val");
    e = _wgetenv_s(&ret, buf, 16, L"tc_wcASE");
    CHECK_EQ_I(e, 0); CHECK_EQ_U(ret, 4); CHECK_WSTR(buf, L"Val");
}
TC_CASE(non_ascii_value)
{
    size_t ret = 99; wchar_t buf[16]; errno_t e;
    static const wchar_t v[] = { 0x00e9, 0x4e2d, 0x0416, 0 };
    wchar_t env[32] = L"TC_WU=";
    env[6] = v[0]; env[7] = v[1]; env[8] = v[2]; env[9] = 0;
    CHECK_EQ_I(_wputenv(env), 0);
    e = _wgetenv_s(&ret, buf, 16, L"TC_WU");
    CHECK_EQ_I(e, 0); CHECK_EQ_U(ret, 4); CHECK_WSTR(buf, v);
}
int main(void)
{
    tc_begin();
    TC_RUN(found_len_plus_1_in_elements); TC_RUN(not_found_zero_and_empty_buf); TC_RUN(size_query_null_buffer);
    TC_RUN(case_insensitive); TC_RUN(non_ascii_value);
    return tc_end();
}
