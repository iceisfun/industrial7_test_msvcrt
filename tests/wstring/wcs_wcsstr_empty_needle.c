/* TC
 * id:    wcs_wcsstr_empty_needle
 * area:  wstring
 * funcs: wcsstr
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.7 (empty needle: returns s1)
 * note:  
 */
#include "tc.h"
#include <wchar.h>
static const wchar_t *volatile ps, *volatile pn;
TC_CASE(nonempty_hay) { static const wchar_t h[] = L"abc"; static const wchar_t n[] = L""; ps = h; pn = n; CHECK_EQ_P(wcsstr(ps, pn), h); }
TC_CASE(both_empty) { static const wchar_t h[] = L""; static const wchar_t n[] = L""; ps = h; pn = n; CHECK_EQ_P(wcsstr(ps, pn), h); }
TC_CASE(empty_hay) { static const wchar_t h[] = L""; static const wchar_t n[] = L"a"; ps = h; pn = n; CHECK(wcsstr(ps, pn) == NULL); }
int main(void) { tc_begin(); TC_RUN(nonempty_hay); TC_RUN(both_empty); TC_RUN(empty_hay); return tc_end(); }
