/* TC
 * id:    pfw_wide_c_lc_hc
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf type field characters: wide c = wchar_t; lc/wc = wchar_t; hc = single-byte char
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static volatile wchar_t wa = L'A';
static volatile int ca = 'q';
TC_CASE(c_ascii) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%c]", (int)wa); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[A]"); CHECK_EQ_I(n, 3); }
TC_CASE(lc) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%lc]", (wint_t)wa); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[A]"); CHECK_EQ_I(n, 3); }
TC_CASE(wc) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%wc]", (wint_t)wa); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[A]"); CHECK_EQ_I(n, 3); }
TC_CASE(hc_ascii) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%hc]", ca); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[q]"); CHECK_EQ_I(n, 3); }
TC_CASE(width_pad_3c_m3c) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%3c][%-3c]", (int)wa, (int)wa); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[  A][A  ]"); CHECK_EQ_I(n, 10); }
int main(void) { tc_begin(); TC_RUN(c_ascii); TC_RUN(lc); TC_RUN(wc); TC_RUN(hc_ascii); TC_RUN(width_pad_3c_m3c); return tc_end(); }
