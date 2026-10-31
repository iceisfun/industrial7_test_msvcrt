/* TC
 * id:    wcs_wcsrchr_nul
 * area:  wstring
 * funcs: wcsrchr
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.5 (terminating null is part of the string)
 * note:  
 */
#include "tc.h"
#include <wchar.h>
static const wchar_t *volatile ps;
TC_CASE(terminator) { static const wchar_t s[] = L"a/b"; ps = s; CHECK_EQ_P(wcsrchr(ps, 0), s + 3); }
TC_CASE(empty) { static const wchar_t s[] = L""; ps = s; CHECK_EQ_P(wcsrchr(ps, 0), s); }
int main(void) { tc_begin(); TC_RUN(terminator); TC_RUN(empty); return tc_end(); }
