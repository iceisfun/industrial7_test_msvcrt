/* TC
 * id:    wcs_wcsrev_basic
 * area:  wstring
 * funcs: _wcsrev
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strrev, _wcsrev, _mbsrev
 */
#include "tc.h"
#include <wchar.h>
#include <string.h>

TC_CASE(even) { wchar_t b[] = L"abcd"; wchar_t *volatile p = b; _wcsrev(p); CHECK_WSTR(b, L"dcba"); }
TC_CASE(odd)  { wchar_t b[] = L"abcde"; wchar_t *volatile p = b; _wcsrev(p); CHECK_WSTR(b, L"edcba"); }
TC_CASE(single) { wchar_t b[] = L"x"; wchar_t *volatile p = b; _wcsrev(p); CHECK_WSTR(b, L"x"); }
TC_CASE(empty) { wchar_t b[] = L""; wchar_t *volatile p = b; wchar_t *r = _wcsrev(p); CHECK(r == b); CHECK_EQ_U(b[0], 0); }
TC_CASE(same_ptr) { wchar_t b[] = L"hello"; wchar_t *volatile p = b; CHECK(_wcsrev(p) == b); }
TC_CASE(palindrome) { wchar_t b[] = L"abcba"; wchar_t *volatile p = b; _wcsrev(p); CHECK_WSTR(b, L"abcba"); }
TC_CASE(double_reverse) { wchar_t b[] = L"Hello, World"; wchar_t *volatile p = b; _wcsrev(p); CHECK_WSTR(b, L"dlroW ,olleH"); _wcsrev(p); CHECK_WSTR(b, L"Hello, World"); }
TC_CASE(terminator_kept) { wchar_t b[] = { L'a', L'b', L'c', 0, L'Z', 0 }; wchar_t *volatile p = b; _wcsrev(p); CHECK_EQ_U(b[3], 0); CHECK_EQ_U(b[4], L'Z'); CHECK_EQ_U(b[0], L'c'); }

int main(void) { tc_begin(); TC_RUN(even); TC_RUN(odd); TC_RUN(single); TC_RUN(empty); TC_RUN(same_ptr); TC_RUN(palindrome); TC_RUN(double_reverse); TC_RUN(terminator_kept); return tc_end(); }
