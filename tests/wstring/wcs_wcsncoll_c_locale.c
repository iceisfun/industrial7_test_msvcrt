/* TC
 * id:    wcs_wcsncoll_c_locale
 * area:  wstring
 * funcs: _wcsncoll _wcsnicoll
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _strncoll, _wcsncoll, _wcsnicoll (at most count characters; "C" locale)
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(n_zero)
{
    const wchar_t * volatile a = L"abc"; const wchar_t * volatile b = L"xyz";
    CHECK_EQ_I(_wcsncoll(a, b, 0), 0); CHECK_EQ_I(_wcsnicoll(a, b, 0), 0);
}
TC_CASE(differ_after_n)
{
    const wchar_t * volatile a = L"abcdef"; const wchar_t * volatile b = L"abcxyz";
    CHECK_EQ_I(_wcsncoll(a, b, 3), 0); CHECK_EQ_I(_wcsnicoll(a, b, 3), 0);
    CHECK(_wcsncoll(a, b, 4) < 0); CHECK(_wcsncoll(b, a, 4) > 0);
    CHECK(_wcsnicoll(a, b, 4) < 0); CHECK(_wcsnicoll(b, a, 4) > 0);
}
TC_CASE(case_insensitive)
{
    const wchar_t * volatile a = L"ABCd"; const wchar_t * volatile b = L"abcD";
    CHECK_EQ_I(_wcsnicoll(a, b, 3), 0); CHECK_EQ_I(_wcsnicoll(a, b, 4), 0); CHECK_EQ_I(_wcsnicoll(a, b, 100), 0);
}
TC_CASE(case_sensitive_differs)
{
    const wchar_t * volatile a = L"ABC"; const wchar_t * volatile b = L"abc";
    CHECK(_wcsncoll(a, b, 3) < 0); CHECK(_wcsncoll(b, a, 3) > 0); CHECK(_wcsncoll(a, b, 1) < 0);
}
TC_CASE(shorter_than_n)
{
    const wchar_t * volatile a = L"ab"; const wchar_t * volatile b = L"abc";
    CHECK(_wcsncoll(a, b, 10) < 0); CHECK(_wcsnicoll(b, a, 10) > 0);
    CHECK_EQ_I(_wcsncoll(a, b, 2), 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(n_zero); TC_RUN(differ_after_n); TC_RUN(case_insensitive); TC_RUN(case_sensitive_differs); TC_RUN(shorter_than_n);
    return tc_end();
}
