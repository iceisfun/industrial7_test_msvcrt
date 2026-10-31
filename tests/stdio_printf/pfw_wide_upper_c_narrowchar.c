/* TC
 * id:    pfw_wide_upper_c_narrowchar
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf type field characters: in wide functions C = single-byte char
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static volatile int cz = 'z';
TC_CASE(C_ascii) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%C]", cz); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[z]"); CHECK_EQ_I(n, 3); }
TC_CASE(C_width) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%4C][%-4C]", cz, cz); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[   z][z   ]"); CHECK_EQ_I(n, 12); }
TC_CASE(C_vs_c) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%C%c%hc%lc", cz, cz, cz, cz); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"zzzz"); CHECK_EQ_I(n, 4); }
int main(void) { tc_begin(); TC_RUN(C_ascii); TC_RUN(C_width); TC_RUN(C_vs_c); return tc_end(); }
