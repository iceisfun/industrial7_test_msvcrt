/* TC
 * id:    wcs_wcsspn_highunits
 * area:  wstring
 * funcs: wcsspn wcscspn
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.2, 7.29.4.5.6
 * note:  
 */
#include "tc.h"
#include <wchar.h>
static const wchar_t *volatile ps, *volatile pn;
TC_CASE(spn_ffff) { static const wchar_t s[] = { 0xFFFF, 0xFFFF, 0xFFFF, L'a', 0 }; static const wchar_t n[] = { 0xFFFF, 0 }; ps = s; pn = n; CHECK_EQ_U(wcsspn(ps, pn), 3); }
TC_CASE(cspn_d800) { static const wchar_t s[] = { L'a', L'b', 0xD800, L'c', 0 }; static const wchar_t n[] = { 0xD800, 0 }; ps = s; pn = n; CHECK_EQ_U(wcscspn(ps, pn), 2); }
TC_CASE(spn_pair) { static const wchar_t s[] = { 0xD83D, 0xDE00, 0xD83D, L'x', 0 }; static const wchar_t n[] = { 0xDE00, 0xD83D, 0 }; ps = s; pn = n; CHECK_EQ_U(wcsspn(ps, pn), 3); }
TC_CASE(no_truncation) { static const wchar_t s[] = { 0x0161, 0 }; static const wchar_t n[] = { 0x0061, 0 }; ps = s; pn = n; CHECK_EQ_U(wcsspn(ps, pn), 0); CHECK_EQ_U(wcscspn(ps, pn), 1); }
int main(void) { tc_begin(); TC_RUN(spn_ffff); TC_RUN(cspn_d800); TC_RUN(spn_pair); TC_RUN(no_truncation); return tc_end(); }
