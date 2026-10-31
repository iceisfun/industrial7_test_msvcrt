/* TC
 * id:    pfw_wide_flags_combo
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 flags - + space # 0; MS docs printf Flag Directives
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(hash_x_o_zero_value) { wchar_t b[64]; volatile unsigned z = 0, v = 255, o = 8; _snwprintf(b, 64, L"%#x|%#o|%#x|%#X|%#o", z, z, v, v, o); CHECK_WSTR(b, L"0|0|0xff|0XFF|010"); }
TC_CASE(plus_zero_d) { wchar_t b[64]; volatile int v = 42, w = -42; _snwprintf(b, 64, L"%+05d|%+05d|%05d", v, w, w); CHECK_WSTR(b, L"+0042|-0042|-0042"); }
TC_CASE(minus_overrides_zero) { wchar_t b[64]; volatile int v = 42; _snwprintf(b, 64, L"[%-05d][%0-5d]", v, v); CHECK_WSTR(b, L"[42   ][42   ]"); }
TC_CASE(space_vs_plus) { wchar_t b[64]; volatile int v = 42; _snwprintf(b, 64, L"[% d][% +d][%+ d]", v, v, v); CHECK_WSTR(b, L"[ 42][+42][+42]"); }
TC_CASE(hash_g_keeps_zeros) { wchar_t b[64]; volatile double v = 1.5, w = 100.0; _snwprintf(b, 64, L"%#g|%g|%#g", v, v, w); CHECK_WSTR(b, L"1.50000|1.5|100.000"); }
TC_CASE(plus_f) { wchar_t b[64]; volatile double v = 3.14159, w = -3.14159; _snwprintf(b, 64, L"%+.3f|%+.3f|% .3f|%08.3f", v, w, v, w); CHECK_WSTR(b, L"+3.142|-3.142| 3.142|-003.142"); }
TC_CASE(hash_f_point) { wchar_t b[64]; volatile double v = 3.0; _snwprintf(b, 64, L"%#.0f|%.0f", v, v); CHECK_WSTR(b, L"3.|3"); }

int main(void) { tc_begin(); TC_RUN(hash_x_o_zero_value); TC_RUN(plus_zero_d); TC_RUN(minus_overrides_zero); TC_RUN(space_vs_plus); TC_RUN(hash_g_keeps_zeros); TC_RUN(plus_f); TC_RUN(hash_f_point); return tc_end(); }
