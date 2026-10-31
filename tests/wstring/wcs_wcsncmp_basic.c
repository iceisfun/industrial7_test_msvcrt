/* TC
 * id:    wcs_wcsncmp_basic
 * area:  wstring
 * funcs: wcsncmp
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.4.3; MS docs strncmp, wcsncmp
 * note:  only the sign of the result is asserted
 */
#include "tc.h"
#include <wchar.h>

static int ncmp(const wchar_t *a, const wchar_t *b, size_t n) { const wchar_t *volatile x = a, *volatile y = b; return wcsncmp(x, y, n); }
TC_CASE(n0) { CHECK_EQ_I(ncmp(L"abc", L"xyz", 0), 0); }
TC_CASE(differ_after) { CHECK_EQ_I(ncmp(L"abcX", L"abcY", 3), 0); }
TC_CASE(differ_within) { CHECK(ncmp(L"abcX", L"abdY", 3) < 0); CHECK(ncmp(L"abd", L"abc", 3) > 0); }
TC_CASE(n_large) { CHECK_EQ_I(ncmp(L"abc", L"abc", 100), 0); }
TC_CASE(stops_nul) { CHECK(ncmp(L"ab", L"abc", 10) < 0); CHECK(ncmp(L"abc", L"ab", 10) > 0); }
TC_CASE(n1) { CHECK_EQ_I(ncmp(L"axx", L"ayy", 1), 0); CHECK(ncmp(L"a", L"b", 1) < 0); }
int main(void) { tc_begin(); TC_RUN(n0); TC_RUN(differ_after); TC_RUN(differ_within); TC_RUN(n_large); TC_RUN(stops_nul); TC_RUN(n1); return tc_end(); }
