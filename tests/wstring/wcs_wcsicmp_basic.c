/* TC
 * id:    wcs_wcsicmp_basic
 * area:  wstring
 * funcs: _wcsicmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _stricmp, _wcsicmp, _mbsicmp
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static int icmp(const wchar_t *a, const wchar_t *b)
{ const wchar_t *volatile pa = a, *volatile pb = b; return _wcsicmp(pa, pb); }
TC_CASE(case_equal) { CHECK_EQ_I(icmp(L"ABC", L"abc"), 0); CHECK_EQ_I(icmp(L"abc", L"ABC"), 0); }
TC_CASE(signs) { CHECK(icmp(L"abc", L"abd") < 0); CHECK(icmp(L"ABD", L"abc") > 0); CHECK(icmp(L"A", L"b") < 0); CHECK(icmp(L"b", L"A") > 0); }
TC_CASE(prefix) { CHECK(icmp(L"abc", L"ABCD") < 0); CHECK(icmp(L"ABCD", L"abc") > 0); }
TC_CASE(empty) { CHECK_EQ_I(icmp(L"", L""), 0); CHECK(icmp(L"", L"a") < 0); CHECK(icmp(L"a", L"") > 0); }
TC_CASE(mixed) { CHECK_EQ_I(icmp(L"HeLLo WoRLD 123", L"hEllO wOrld 123"), 0); }
int main(void) { tc_begin(); TC_RUN(case_equal); TC_RUN(signs); TC_RUN(prefix); TC_RUN(empty); TC_RUN(mixed); return tc_end(); }
