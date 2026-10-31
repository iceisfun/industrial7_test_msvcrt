/* TC
 * id:    wcs_wcslen_basic
 * area:  wstring
 * funcs: wcslen
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.6.1; MS docs strlen, wcslen
 * note:  wchar_t units, not code points
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(empty) { static const wchar_t s[] = L""; const wchar_t *volatile p = s; CHECK_EQ_U(wcslen(p), 0); }
TC_CASE(ascii) { static const wchar_t s[] = L"hello"; const wchar_t *volatile p = s; CHECK_EQ_U(wcslen(p), 5); }
TC_CASE(embedded) { static const wchar_t s[] = { 'a', 'b', 0, 'c', 'd', 0 }; const wchar_t *volatile p = s; CHECK_EQ_U(wcslen(p), 2); }
TC_CASE(high_units) { static const wchar_t s[] = { 0xFFFF, 0x8000, 0x7FFF, 1, 0 }; const wchar_t *volatile p = s; CHECK_EQ_U(wcslen(p), 4); }
TC_CASE(long1000)
{
    static wchar_t s[1001]; const wchar_t *volatile p = s; int i;
    for (i = 0; i < 1000; i++) s[i] = (wchar_t)('a' + i % 26);
    s[1000] = 0;
    CHECK_EQ_U(wcslen(p), 1000);
}
int main(void) { tc_begin(); TC_RUN(empty); TC_RUN(ascii); TC_RUN(embedded); TC_RUN(high_units); TC_RUN(long1000); return tc_end(); }
