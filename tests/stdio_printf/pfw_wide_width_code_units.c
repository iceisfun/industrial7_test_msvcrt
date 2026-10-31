/* TC
 * id:    pfw_wide_width_code_units
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf width: minimum number of characters written; never truncates
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static const wchar_t * volatile ab = L"abcd";
TC_CASE(w5c_bmp) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%5c", 0x4E2D); CHECK_EQ_I(n, 5); CHECK(b[3]==L' ' && b[0]==L' '); CHECK_EQ_U(b[4], 0x4E2D); CHECK_EQ_U(b[5], 0); }
TC_CASE(w2s_longer_than_width_no_truncate) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%2s", ab); b[n < 0 ? 0 : n] = 0; CHECK_EQ_I(n, 4); CHECK_WSTR(b, L"abcd"); }
int main(void) { tc_begin(); TC_RUN(w5c_bmp); TC_RUN(w2s_longer_than_width_no_truncate); return tc_end(); }
