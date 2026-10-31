/* TC
 * id:    pfw_wide_precision_code_units
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf precision for s: maximum number of characters printed
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static const wchar_t * volatile ab = L"abcdef";
TC_CASE(p3s) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%.3s", ab); b[n < 0 ? 0 : n] = 0; CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"abc"); }
TC_CASE(p0s_empty) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%.0s]", ab); b[n < 0 ? 0 : n] = 0; CHECK_EQ_I(n, 2); CHECK_WSTR(b, L"[]"); }
TC_CASE(p_star_s) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%.*s", 4, ab); b[n < 0 ? 0 : n] = 0; CHECK_EQ_I(n, 4); CHECK_WSTR(b, L"abcd"); }
int main(void) { tc_begin(); TC_RUN(p3s); TC_RUN(p0s_empty); TC_RUN(p_star_s); return tc_end(); }
