/* TC
 * id:    wcs_wcscoll_c_locale
 * area:  wstring
 * funcs: wcscoll
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcoll, wcscoll (collation uses LC_COLLATE; "C" locale compares code units like wcscmp)
 */
#include "tc.h"
#include <wchar.h>

static int sgn(int v) { return (v > 0) - (v < 0); }

TC_CASE(equal) { const wchar_t * volatile a = L"hello"; const wchar_t * volatile b = L"hello"; CHECK_EQ_I(wcscoll(a, b), 0); }
TC_CASE(ordering)
{
    const wchar_t * volatile a = L"abc"; const wchar_t * volatile b = L"abd"; const wchar_t * volatile c = L"ab";
    CHECK(wcscoll(a, b) < 0); CHECK(wcscoll(b, a) > 0);
    CHECK(wcscoll(c, a) < 0); CHECK(wcscoll(a, c) > 0);
    CHECK_EQ_I(sgn(wcscoll(a, b)), sgn(wcscmp(a, b)));
    CHECK_EQ_I(sgn(wcscoll(a, c)), sgn(wcscmp(a, c)));
}
TC_CASE(high_unit)
{
    wchar_t hi[2] = { 0xFFFF, 0 }; wchar_t lo[2] = { L'a', 0 };
    const wchar_t * volatile a = hi; const wchar_t * volatile b = lo;
    CHECK(wcscoll(a, b) > 0); CHECK(wcscoll(b, a) < 0);
}
TC_CASE(empty)
{
    const wchar_t * volatile e = L""; const wchar_t * volatile a = L"a";
    CHECK_EQ_I(wcscoll(e, e), 0); CHECK(wcscoll(e, a) < 0); CHECK(wcscoll(a, e) > 0);
}
TC_CASE(case_sensitive)
{
    const wchar_t * volatile u = L"ABC"; const wchar_t * volatile l = L"abc";
    CHECK(wcscoll(u, l) < 0); CHECK(wcscoll(l, u) > 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(equal); TC_RUN(ordering); TC_RUN(high_unit); TC_RUN(empty); TC_RUN(case_sensitive);
    return tc_end();
}
