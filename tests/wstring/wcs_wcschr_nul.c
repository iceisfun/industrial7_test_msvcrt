/* TC
 * id:    wcs_wcschr_nul
 * area:  wstring
 * funcs: wcschr wcsrchr
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.1, 7.29.4.5.5: the terminating null wide character is considered part of the string
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const wchar_t *volatile vs;
TC_CASE(chr_nul) { static const wchar_t s[] = L"abc"; vs = s; CHECK_EQ_P(wcschr(vs, 0), s + 3); }
TC_CASE(chr_empty) { static const wchar_t s[] = L""; vs = s; CHECK_EQ_P(wcschr(vs, 0), s); }
TC_CASE(rchr_nul) { static const wchar_t s[] = L"abc"; vs = s; CHECK_EQ_P(wcsrchr(vs, 0), s + 3); }
TC_CASE(rchr_empty) { static const wchar_t s[] = L""; vs = s; CHECK_EQ_P(wcsrchr(vs, 0), s); }
int main(void) { tc_begin(); TC_RUN(chr_nul); TC_RUN(chr_empty); TC_RUN(rchr_nul); TC_RUN(rchr_empty); return tc_end(); }
