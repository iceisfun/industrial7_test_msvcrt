/* TC
 * id:    wcs_wcscpy_basic
 * area:  wstring
 * funcs: wcscpy
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.2.1; MS docs strcpy, wcscpy
 * note:  
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(ret_dest) { wchar_t d[8]; static const wchar_t s[] = L"abc"; const wchar_t *volatile p = s; CHECK_EQ_P(wcscpy(d, p), d); CHECK_WSTR(d, L"abc"); }
TC_CASE(empty) { wchar_t d[4] = { 'x', 'y', 'z', 0 }; static const wchar_t s[] = L""; const wchar_t *volatile p = s; wcscpy(d, p); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(d[1], 'y'); }
TC_CASE(exact_fit) { wchar_t d[4]; static const wchar_t s[] = L"abc"; const wchar_t *volatile p = s; wcscpy(d, p); CHECK_EQ_I(d[3], 0); CHECK_WSTR(d, L"abc"); }
TC_CASE(high_units) { wchar_t d[8]; static const wchar_t s[] = { 0xFFFF, 0x8000, 0xD83D, 0xDE00, 0 }; const wchar_t *volatile p = s;
    wcscpy(d, p); CHECK_EQ_I(d[0], 0xFFFF); CHECK_EQ_I(d[1], 0x8000); CHECK_EQ_I(d[2], 0xD83D); CHECK_EQ_I(d[3], 0xDE00); CHECK_EQ_I(d[4], 0); }
TC_CASE(tail_untouched) { wchar_t d[8]; int i; static const wchar_t s[] = L"ab"; const wchar_t *volatile p = s;
    for (i = 0; i < 8; i++) d[i] = 0x5555;
    wcscpy(d, p);
    CHECK_EQ_I(d[2], 0); for (i = 3; i < 8; i++) CHECK_EQ_I(d[i], 0x5555); }
int main(void) { tc_begin(); TC_RUN(ret_dest); TC_RUN(empty); TC_RUN(exact_fit); TC_RUN(high_units); TC_RUN(tail_untouched); return tc_end(); }
