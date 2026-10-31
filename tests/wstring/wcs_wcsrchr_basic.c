/* TC
 * id:    wcs_wcsrchr_basic
 * area:  wstring
 * funcs: wcsrchr wcschr
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.5; MS docs strrchr, wcsrchr
 * note:  
 */
#include "tc.h"
#include <wchar.h>
static const wchar_t *volatile ps;
TC_CASE(last_of_several) { static const wchar_t s[] = L"a/b/c/d"; ps = s; CHECK_EQ_P(wcsrchr(ps, L'/'), s + 5); }
TC_CASE(not_found) { static const wchar_t s[] = L"abc"; ps = s; CHECK(wcsrchr(ps, L'x') == NULL); }
TC_CASE(single) { static const wchar_t s[] = L"abcd"; ps = s; CHECK_EQ_P(wcsrchr(ps, L'c'), wcschr(ps, L'c')); CHECK_EQ_P(wcsrchr(ps, L'c'), s + 2); }
TC_CASE(last_index) { static const wchar_t s[] = L"xxxy"; ps = s; CHECK_EQ_P(wcsrchr(ps, L'y'), s + 3); }
TC_CASE(index0) { static const wchar_t s[] = L"qabc"; ps = s; CHECK_EQ_P(wcsrchr(ps, L'q'), s); }
TC_CASE(empty) { static const wchar_t s[] = L""; ps = s; CHECK(wcsrchr(ps, L'a') == NULL); }
int main(void) { tc_begin(); TC_RUN(last_of_several); TC_RUN(not_found); TC_RUN(single); TC_RUN(last_index); TC_RUN(index0); TC_RUN(empty); return tc_end(); }
