/* TC
 * id:    wcs_wcscspn_basic
 * area:  wstring
 * funcs: wcscspn
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.2; MS docs strcspn, wcscspn
 * note:  
 */
#include "tc.h"
#include <wchar.h>
static const wchar_t *volatile ps, *volatile pn;
static size_t F(const wchar_t *h, const wchar_t *n) { ps = h; pn = n; return wcscspn(ps, pn); }
TC_CASE(first_reject) { CHECK_EQ_U(F(L"hello world", L" ,"), 5); }
TC_CASE(no_reject) { CHECK_EQ_U(F(L"hello", L"xyz"), 5); }
TC_CASE(reject_at0) { CHECK_EQ_U(F(L"hello", L"h"), 0); }
TC_CASE(empty_set) { CHECK_EQ_U(F(L"hello", L""), 5); }
TC_CASE(empty_str) { CHECK_EQ_U(F(L"", L"abc"), 0); }
int main(void) { tc_begin(); TC_RUN(first_reject); TC_RUN(no_reject); TC_RUN(reject_at0); TC_RUN(empty_set); TC_RUN(empty_str); return tc_end(); }
