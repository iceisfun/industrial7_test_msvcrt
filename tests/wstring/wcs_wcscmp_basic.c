/* TC
 * id:    wcs_wcscmp_basic
 * area:  wstring
 * funcs: wcscmp
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.4.1; MS docs strcmp, wcscmp
 * note:  only the sign of the result is asserted
 */
#include "tc.h"
#include <wchar.h>

static int cmp(const wchar_t *a, const wchar_t *b) { const wchar_t *volatile x = a, *volatile y = b; return wcscmp(x, y); }
TC_CASE(equal) { CHECK_EQ_I(cmp(L"abc", L"abc"), 0); }
TC_CASE(less) { CHECK(cmp(L"abc", L"abd") < 0); CHECK(cmp(L"abc", L"b") < 0); }
TC_CASE(greater) { CHECK(cmp(L"abd", L"abc") > 0); CHECK(cmp(L"b", L"abc") > 0); }
TC_CASE(prefix) { CHECK(cmp(L"ab", L"abc") < 0); CHECK(cmp(L"abc", L"ab") > 0); }
TC_CASE(empty_vs) { CHECK(cmp(L"", L"a") < 0); CHECK(cmp(L"a", L"") > 0); }
TC_CASE(empty_empty) { CHECK_EQ_I(cmp(L"", L""), 0); }
TC_CASE(case_sens) { CHECK(cmp(L"A", L"a") < 0); CHECK(cmp(L"a", L"A") > 0); }
int main(void) { tc_begin(); TC_RUN(equal); TC_RUN(less); TC_RUN(greater); TC_RUN(prefix); TC_RUN(empty_vs); TC_RUN(empty_empty); TC_RUN(case_sens); return tc_end(); }
