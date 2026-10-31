/* TC
 * id:    wcs_wcsstr_highunits
 * area:  wstring
 * funcs: wcsstr
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.7 (wchar_t units compared, no code-point awareness)
 * note:  UTF-16 code units are compared individually.
 */
#include "tc.h"
#include <wchar.h>
static const wchar_t *volatile ps, *volatile pn;
static const wchar_t h[] = { L'a', 0xD83D, 0xDE00, L'b', 0xFFFF, L'c', 0 };
TC_CASE(pair) { static const wchar_t n[] = { 0xD83D, 0xDE00, 0 }; ps = h; pn = n; CHECK_EQ_P(wcsstr(ps, pn), h + 1); }
TC_CASE(lone_trail) { static const wchar_t n[] = { 0xDE00, L'b', 0 }; ps = h; pn = n; CHECK_EQ_P(wcsstr(ps, pn), h + 2); }
TC_CASE(ffff) { static const wchar_t n[] = { 0xFFFF, L'c', 0 }; ps = h; pn = n; CHECK_EQ_P(wcsstr(ps, pn), h + 4); }
TC_CASE(lone_lead_before_wrong) { static const wchar_t n[] = { 0xD83D, L'b', 0 }; ps = h; pn = n; CHECK(wcsstr(ps, pn) == NULL); }
int main(void) { tc_begin(); TC_RUN(pair); TC_RUN(lone_trail); TC_RUN(ffff); TC_RUN(lone_lead_before_wrong); return tc_end(); }
