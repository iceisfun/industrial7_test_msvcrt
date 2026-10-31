/* TC
 * id:    wcs_wcslen_surrogates
 * area:  wstring
 * funcs: wcslen
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strlen, wcslen, _mbslen
 * note:  wcslen counts 16-bit wchar_t units, not code points
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(pair) { static const wchar_t s[] = { 0xD83D, 0xDE00, 0 }; const wchar_t *volatile p = s; CHECK_EQ_U(wcslen(p), 2); }
TC_CASE(lone_high) { static const wchar_t s[] = { 0xD800, 0 }; const wchar_t *volatile p = s; CHECK_EQ_U(wcslen(p), 1); }
TC_CASE(lone_low_then_high) { static const wchar_t s[] = { 0xDC00, 0xDBFF, 0 }; const wchar_t *volatile p = s; CHECK_EQ_U(wcslen(p), 2); }
TC_CASE(bmp_max) { static const wchar_t s[] = { 0xFFFF, 0 }; const wchar_t *volatile p = s; CHECK_EQ_U(wcslen(p), 1); }
TC_CASE(bom) { static const wchar_t s[] = { 0xFEFF, 'x', 0 }; const wchar_t *volatile p = s; CHECK_EQ_U(wcslen(p), 2); }
int main(void) { tc_begin(); TC_RUN(pair); TC_RUN(lone_high); TC_RUN(lone_low_then_high); TC_RUN(bmp_max); TC_RUN(bom); return tc_end(); }
