/* TC
 * id:    wcs_wcsicoll_c_locale
 * area:  wstring
 * funcs: _wcsicoll
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _stricoll, _wcsicoll (case-insensitive collation; "C" locale)
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(case_fold)
{
    const wchar_t * volatile a = L"ABC"; const wchar_t * volatile b = L"abc"; const wchar_t * volatile c = L"aBc";
    CHECK_EQ_I(_wcsicoll(a, b), 0); CHECK_EQ_I(_wcsicoll(b, a), 0); CHECK_EQ_I(_wcsicoll(a, c), 0);
}
TC_CASE(ordering)
{
    const wchar_t * volatile a = L"ABC"; const wchar_t * volatile b = L"abd"; const wchar_t * volatile c = L"AB";
    CHECK(_wcsicoll(a, b) < 0); CHECK(_wcsicoll(b, a) > 0);
    CHECK(_wcsicoll(c, a) < 0); CHECK(_wcsicoll(a, c) > 0);
}
TC_CASE(empty)
{
    const wchar_t * volatile e = L""; const wchar_t * volatile a = L"A";
    CHECK_EQ_I(_wcsicoll(e, e), 0); CHECK(_wcsicoll(e, a) < 0); CHECK(_wcsicoll(a, e) > 0);
}
int main(void) { tc_begin(); TC_RUN(case_fold); TC_RUN(ordering); TC_RUN(empty); return tc_end(); }
