/* TC
 * id:    wcs_wcspbrk_highunits
 * area:  wstring
 * funcs: wcspbrk
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.4
 * note:  
 */
#include "tc.h"
#include <wchar.h>
static const wchar_t *volatile ps, *volatile pn;
TC_CASE(ffff) { static const wchar_t s[] = { L'a', L'b', 0xFFFF, L'c', 0 }; static const wchar_t n[] = { L'x', 0xFFFF, 0 }; ps = s; pn = n; CHECK_EQ_P(wcspbrk(ps, pn), s + 2); }
TC_CASE(surrogates) { static const wchar_t s[] = { L'a', 0xD83D, 0xDE00, 0 }; static const wchar_t n[] = { 0xDE00, 0xD83D, 0 }; ps = s; pn = n; CHECK_EQ_P(wcspbrk(ps, pn), s + 1); }
TC_CASE(no_truncation) { static const wchar_t s[] = { 0x0161, L'b', 0 }; static const wchar_t n[] = { 0x0061, 0 }; ps = s; pn = n; CHECK(wcspbrk(ps, pn) == NULL); }
int main(void) { tc_begin(); TC_RUN(ffff); TC_RUN(surrogates); TC_RUN(no_truncation); return tc_end(); }
