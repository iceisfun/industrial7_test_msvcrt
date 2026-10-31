/* TC
 * id:    pfw_wide_star_width_prec
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 p5-6 (negative field width is - flag + positive width; negative precision taken as omitted)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static volatile int w4 = 4, wn4 = -4, pn1 = -1, w8 = 8, p3 = 3;
static volatile double pi = 3.14159;
static const wchar_t * volatile s = L"abc";
TC_CASE(star_d) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%*d]", w4, 7); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[   7]"); CHECK_EQ_I(n, 6); }
TC_CASE(minus_star_d) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%-*d]", w4, 7); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[7   ]"); }
TC_CASE(negative_width_means_left) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%*d]", wn4, 7); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[7   ]"); CHECK_EQ_I(n, 6); }
TC_CASE(star_prec_s) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%.*s]", 2, s); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[ab]"); }
TC_CASE(negative_precision_ignored) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%.*s][%.*d]", pn1, s, pn1, 5); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[abc][5]"); }
TC_CASE(star_star_f) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%*.*f]", w8, p3, (double)pi); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[   3.142]"); CHECK_EQ_I(n, 10); }
int main(void) { tc_begin(); TC_RUN(star_d); TC_RUN(minus_star_d); TC_RUN(negative_width_means_left); TC_RUN(star_prec_s); TC_RUN(negative_precision_ignored); TC_RUN(star_star_f); return tc_end(); }
