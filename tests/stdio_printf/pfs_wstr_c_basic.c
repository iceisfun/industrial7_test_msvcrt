/* TC
 * id:    pfs_wstr_c_basic
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf type field characters (wprintf: c = wide char, C/hc = narrow char)

 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile wchar_t wA = L'A', euro = 0x20AC;
static volatile char cB = 'B';

TC_CASE(c)  { wchar_t w[16]; int n = _snwprintf(w, 16, L"%c", wA); CHECK_WSTR(w, L"A"); CHECK_EQ_I(n, 1); }
TC_CASE(lc_wc) { wchar_t w[16]; _snwprintf(w, 16, L"%lc%wc", wA, wA); CHECK_WSTR(w, L"AA"); }
TC_CASE(C)  { wchar_t w[16]; _snwprintf(w, 16, L"%C", (int)cB); CHECK_WSTR(w, L"B"); }
TC_CASE(hc) { wchar_t w[16]; _snwprintf(w, 16, L"%hc", (int)cB); CHECK_WSTR(w, L"B"); }
TC_CASE(width) { wchar_t w[16]; _snwprintf(w, 16, L"[%3c][%-3c]", wA, wA); CHECK_WSTR(w, L"[  A][A  ]"); }
TC_CASE(euro) { wchar_t w[16]; int n = _snwprintf(w, 16, L"%c", euro); CHECK_EQ_I(n, 1); CHECK_EQ_U(w[0], 0x20AC); }

int main(void) { tc_begin(); TC_RUN(c); TC_RUN(lc_wc); TC_RUN(C); TC_RUN(hc); TC_RUN(width); TC_RUN(euro); return tc_end(); }
