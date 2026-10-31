/* TC
 * id:    wcs_wcspbrk_basic
 * area:  wstring
 * funcs: wcspbrk
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.4; MS docs strpbrk, wcspbrk
 * note:  
 */
#include "tc.h"
#include <wchar.h>
static const wchar_t *volatile ps, *volatile pn;
static const wchar_t *F(const wchar_t *h, const wchar_t *n) { ps = h; pn = n; return wcspbrk(ps, pn); }
TC_CASE(first) { static const wchar_t s[] = L"hello world"; CHECK_EQ_P(F(s, L"ow"), s + 4); }
TC_CASE(none) { CHECK(F(L"hello", L"xyz") == NULL); }
TC_CASE(empty_set) { CHECK(F(L"hello", L"") == NULL); }
TC_CASE(empty_str) { CHECK(F(L"", L"abc") == NULL); }
TC_CASE(at0) { static const wchar_t s[] = L"hello"; CHECK_EQ_P(F(s, L"xh"), s); }
TC_CASE(order) { static const wchar_t s[] = L"abcdef"; CHECK_EQ_P(F(s, L"fdb"), s + 1); CHECK_EQ_P(F(s, L"bdf"), s + 1); }
int main(void) { tc_begin(); TC_RUN(first); TC_RUN(none); TC_RUN(empty_set); TC_RUN(empty_str); TC_RUN(at0); TC_RUN(order); return tc_end(); }
