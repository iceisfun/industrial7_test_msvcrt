/* TC
 * id:    wcs_wcsspn_basic
 * area:  wstring
 * funcs: wcsspn
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.6; MS docs strspn, wcsspn
 * note:  
 */
#include "tc.h"
#include <wchar.h>
static const wchar_t *volatile ps, *volatile pn;
static size_t F(const wchar_t *h, const wchar_t *n) { ps = h; pn = n; return wcsspn(ps, pn); }
TC_CASE(prefix) { CHECK_EQ_U(F(L"aabbcx", L"abc"), 5); }
TC_CASE(none) { CHECK_EQ_U(F(L"xyz", L"abc"), 0); }
TC_CASE(entire) { CHECK_EQ_U(F(L"abcabc", L"cba"), 6); }
TC_CASE(empty_str) { CHECK_EQ_U(F(L"", L"abc"), 0); }
TC_CASE(empty_set) { CHECK_EQ_U(F(L"abc", L""), 0); }
TC_CASE(big_set) { CHECK_EQ_U(F(L"ab", L"zyxwvutsrqponmlkjihgfedcba"), 2); }
int main(void) { tc_begin(); TC_RUN(prefix); TC_RUN(none); TC_RUN(entire); TC_RUN(empty_str); TC_RUN(empty_set); TC_RUN(big_set); return tc_end(); }
