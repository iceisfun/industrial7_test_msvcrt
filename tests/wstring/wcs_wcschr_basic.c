/* TC
 * id:    wcs_wcschr_basic
 * area:  wstring
 * funcs: wcschr
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.1; MS docs wcschr
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static wchar_t *chr(const wchar_t *s, wchar_t c) { const wchar_t *volatile p = s; volatile wchar_t vc = c; return wcschr(p, vc); }
TC_CASE(first_of_several) { static const wchar_t s[] = L"abcabc"; CHECK_EQ_P(chr(s, 'b'), s + 1); }
TC_CASE(not_found) { static const wchar_t s[] = L"abc"; CHECK(chr(s, 'z') == NULL); }
TC_CASE(pointer_arith) { static const wchar_t s[] = L"hello world"; wchar_t *p = chr(s, 'w'); CHECK_EQ_I(p - s, 6); CHECK_EQ_I(*p, 'w'); }
TC_CASE(index0) { static const wchar_t s[] = L"xyz"; CHECK_EQ_P(chr(s, 'x'), s); }
TC_CASE(empty) { static const wchar_t s[] = L""; CHECK(chr(s, 'a') == NULL); }
int main(void) { tc_begin(); TC_RUN(first_of_several); TC_RUN(not_found); TC_RUN(pointer_arith); TC_RUN(index0); TC_RUN(empty); return tc_end(); }
