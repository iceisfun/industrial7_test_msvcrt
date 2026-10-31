/* TC
 * id:    pfw_wide_percent_percent
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 '%%' conversion; MS docs printf
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(single) { wchar_t b[16]; int n = _snwprintf(b, 16, L"%%"); CHECK_WSTR(b, L"%"); CHECK_EQ_I(n, 1); }
TC_CASE(double_pairs) { wchar_t b[16]; _snwprintf(b, 16, L"%%%%"); CHECK_WSTR(b, L"%%"); }
TC_CASE(adjacent_to_conversion) { wchar_t b[32]; volatile int v = 50; _snwprintf(b, 32, L"%d%%|%%%d|%%d", v, v); CHECK_WSTR(b, L"50%|%50|%d"); }
TC_CASE(return_count) { wchar_t b[32]; volatile int v = 100; int n = _snwprintf(b, 32, L"%d%%", v); CHECK_EQ_I(n, 4); CHECK_WSTR(b, L"100%"); }

int main(void) { tc_begin(); TC_RUN(single); TC_RUN(double_pairs); TC_RUN(adjacent_to_conversion); TC_RUN(return_count); return tc_end(); }
