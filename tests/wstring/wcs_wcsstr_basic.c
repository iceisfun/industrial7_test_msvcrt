/* TC
 * id:    wcs_wcsstr_basic
 * area:  wstring
 * funcs: wcsstr
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.7; MS docs strstr, wcsstr
 * note:  
 */
#include "tc.h"
#include <wchar.h>
static const wchar_t *volatile ps, *volatile pn;
static const wchar_t *F(const wchar_t *h, const wchar_t *n) { ps = h; pn = n; return wcsstr(ps, pn); }
TC_CASE(middle) { static const wchar_t h[] = L"hello world"; CHECK_EQ_P(F(h, L"lo w"), h + 3); }
TC_CASE(not_found) { CHECK(F(L"hello", L"xyz") == NULL); CHECK(F(L"hello", L"hellp") == NULL); }
TC_CASE(start) { static const wchar_t h[] = L"hello"; CHECK_EQ_P(F(h, L"he"), h); }
TC_CASE(end) { static const wchar_t h[] = L"hello"; CHECK_EQ_P(F(h, L"llo"), h + 2); CHECK_EQ_P(F(h, L"o"), h + 4); }
TC_CASE(longer) { CHECK(F(L"ab", L"abc") == NULL); }
TC_CASE(overlap) { static const wchar_t h[] = L"aaab"; CHECK_EQ_P(F(h, L"aab"), h + 1); }
TC_CASE(first) { static const wchar_t h[] = L"abXabXab"; CHECK_EQ_P(F(h, L"ab"), h); CHECK_EQ_P(F(h, L"bX"), h + 1); }
int main(void) { tc_begin(); TC_RUN(middle); TC_RUN(not_found); TC_RUN(start); TC_RUN(end); TC_RUN(longer); TC_RUN(overlap); TC_RUN(first); return tc_end(); }
