/* TC
 * id:    wcs_wcsnicmp_basic
 * area:  wstring
 * funcs: _wcsnicmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strnicmp, _wcsnicmp
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static int nicmp(const wchar_t *a, const wchar_t *b, size_t n)
{ const wchar_t *volatile pa = a, *volatile pb = b; volatile size_t vn = n; return _wcsnicmp(pa, pb, vn); }
TC_CASE(n0) { CHECK_EQ_I(nicmp(L"abc", L"xyz", 0), 0); }
TC_CASE(differ_after_n) { CHECK_EQ_I(nicmp(L"abcX", L"ABCY", 3), 0); CHECK(nicmp(L"abcX", L"ABCY", 4) < 0); }
TC_CASE(case_within_n) { CHECK_EQ_I(nicmp(L"HeLLo", L"hEllO", 5), 0); }
TC_CASE(n_larger) { CHECK_EQ_I(nicmp(L"AbC", L"aBc", 100), 0); CHECK(nicmp(L"ab", L"ABC", 100) < 0); }
TC_CASE(stops_at_nul) { static const wchar_t a[] = { 'a', 0, 'x', 0 }, b[] = { 'A', 0, 'y', 0 }; CHECK_EQ_I(nicmp(a, b, 3), 0); }
TC_CASE(signs) { CHECK(nicmp(L"b", L"A", 1) > 0); CHECK(nicmp(L"A", L"b", 1) < 0); }
int main(void) { tc_begin(); TC_RUN(n0); TC_RUN(differ_after_n); TC_RUN(case_within_n); TC_RUN(n_larger); TC_RUN(stops_at_nul); TC_RUN(signs); return tc_end(); }
