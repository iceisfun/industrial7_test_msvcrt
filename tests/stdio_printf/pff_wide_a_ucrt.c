/* TC
 * id:    pff_wide_a_ucrt
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.1 / 7.21.6.1 (a, A); MS docs printf format specification
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define N(b) (sizeof(b) / sizeof((b)[0]))
TC_CASE(a1)   { wchar_t b[32]; volatile double v = 1.5; int n = _snwprintf(b, N(b), L"%.1a", v); CHECK_WSTR(b, L"0x1.8p+0"); CHECK_EQ_I(n, 8); }
TC_CASE(A2)   { wchar_t b[32]; volatile double v = 255.0; _snwprintf(b, N(b), L"%.2A", v); CHECK_WSTR(b, L"0X1.FEP+7"); }
TC_CASE(a0)   { wchar_t b[32]; volatile double v = 1.0; _snwprintf(b, N(b), L"%.0a", v); CHECK_WSTR(b, L"0x1p+0"); }
int main(void) { tc_begin(); TC_RUN(a1); TC_RUN(A2); TC_RUN(a0); return tc_end(); }
