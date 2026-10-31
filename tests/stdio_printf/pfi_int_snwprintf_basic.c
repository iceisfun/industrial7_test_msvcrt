/* TC
 * id:    pfi_int_snwprintf_basic
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.3 / 7.21.6.1 conversions; MS docs _snprintf, _snwprintf
 * note:  swprintf is inline in ucrt and has two signatures in old CRTs, so _snwprintf is the common wide entry.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(dec) { wchar_t b[64]; int n = _snwprintf(b, 64, L"%d|%i|%u", -42, 7, 4000000000u); CHECK_WSTR(b, L"-42|7|4000000000"); CHECK_EQ_I(n, 16); }
TC_CASE(hexoct) { wchar_t b[64]; int n = _snwprintf(b, 64, L"%x|%X|%o", 255, 255, 8); CHECK_WSTR(b, L"ff|FF|10"); CHECK_EQ_I(n, 8); }
TC_CASE(neg_to_unsigned)
{
    wchar_t b[64]; volatile int m = -1;
    _snwprintf(b, 64, L"%u|%x|%o", m, m, m);
    CHECK_WSTR(b, L"4294967295|ffffffff|37777777777");
}
TC_CASE(nul_when_room)
{
    wchar_t b[8]; int n, i; for (i = 0; i < 8; i++) b[i] = L'Z';
    n = _snwprintf(b, 8, L"%d", 123);
    CHECK_EQ_I(n, 3); CHECK(b[3] == 0); CHECK(b[4] == L'Z');
}
TC_CASE(zero) { wchar_t b[8]; int n = _snwprintf(b, 8, L"%d%u%x", 0, 0u, 0); CHECK_WSTR(b, L"000"); CHECK_EQ_I(n, 3); }
int main(void) { tc_begin(); TC_RUN(dec); TC_RUN(hexoct); TC_RUN(neg_to_unsigned); TC_RUN(nul_when_room); TC_RUN(zero); return tc_end(); }
