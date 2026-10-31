/* TC
 * id:    pfw_wide_c_bmp
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf: %c in wide function writes a wchar_t unchanged
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static volatile unsigned int cp;
static int one(wchar_t *b, unsigned v) { int n; fill(b); cp = v; n = _snwprintf(b, 63, L"%c", (int)cp); return n; }
TC_CASE(U_00E9) { wchar_t b[64]; int n = one(b, 0xE9); CHECK_EQ_I(n, 1); CHECK_EQ_U(b[0], 0xE9); CHECK_EQ_U(b[1], 0); }
TC_CASE(U_4E2D) { wchar_t b[64]; int n = one(b, 0x4E2D); CHECK_EQ_I(n, 1); CHECK_EQ_U(b[0], 0x4E2D); CHECK_EQ_U(b[1], 0); }
TC_CASE(U_FFFF) { wchar_t b[64]; int n = one(b, 0xFFFF); CHECK_EQ_I(n, 1); CHECK_EQ_U(b[0], 0xFFFF); CHECK_EQ_U(b[1], 0); }
TC_CASE(U_0001_control) { wchar_t b[64]; int n = one(b, 1); CHECK_EQ_I(n, 1); CHECK_EQ_U(b[0], 1); CHECK_EQ_U(b[1], 0); }
TC_CASE(return_counts_1_each) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%c%c%c", 0xE9, 0x4E2D, 0xFFFF); CHECK_EQ_I(n, 3); CHECK_EQ_U(b[0], 0xE9); CHECK_EQ_U(b[1], 0x4E2D); CHECK_EQ_U(b[2], 0xFFFF); }
int main(void) { tc_begin(); TC_RUN(U_00E9); TC_RUN(U_4E2D); TC_RUN(U_FFFF); TC_RUN(U_0001_control); TC_RUN(return_counts_1_each); return tc_end(); }
