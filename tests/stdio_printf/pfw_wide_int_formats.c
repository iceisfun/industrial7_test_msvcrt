/* TC
 * id:    pfw_wide_int_formats
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 d i u x X o with flags; length h/l
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static volatile int v42 = 42, v255 = 255, v8 = 8, vmin = (-2147483647 - 1), v65537 = 65537, v70000 = 70000;
TC_CASE(decimal) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"%d|%i|%u", -v42, v42, 4000000000u); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"-42|42|4000000000"); CHECK_EQ_I(n, (int)(sizeof(L"-42|42|4000000000")/sizeof(wchar_t)) - 1); }
TC_CASE(hex_oct_alt) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"%x|%X|%o|%#x|%#X|%#o", v255, v255, v255, v255, v255, v8); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"ff|FF|377|0xff|0XFF|010"); CHECK_EQ_I(n, (int)(sizeof(L"ff|FF|377|0xff|0XFF|010")/sizeof(wchar_t)) - 1); }
TC_CASE(width_zero_pad) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"[%5d][%-5d][%05d][%#06x]", v42, v42, v42, v255); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[   42][42   ][00042][0x00ff]"); CHECK_EQ_I(n, (int)(sizeof(L"[   42][42   ][00042][0x00ff]")/sizeof(wchar_t)) - 1); }
TC_CASE(precision_min_digits) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"[%.5d][%8.3d][%.0d]", v42, 7, 0); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[00042][     007][]"); CHECK_EQ_I(n, (int)(sizeof(L"[00042][     007][]")/sizeof(wchar_t)) - 1); }
TC_CASE(plus_space) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"[%+d][% d][% d]", v42, v42, -v42); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[+42][ 42][-42]"); CHECK_EQ_I(n, (int)(sizeof(L"[+42][ 42][-42]")/sizeof(wchar_t)) - 1); }
TC_CASE(int_min) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"%d|%x", vmin, vmin); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"-2147483648|80000000"); CHECK_EQ_I(n, (int)(sizeof(L"-2147483648|80000000")/sizeof(wchar_t)) - 1); }
TC_CASE(hd_truncation_ld) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"%hd|%hd|%ld|%ld", v65537, v70000, (long)v65537, (long)v70000); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"1|4464|65537|70000"); CHECK_EQ_I(n, (int)(sizeof(L"1|4464|65537|70000")/sizeof(wchar_t)) - 1); }
int main(void) { tc_begin(); TC_RUN(decimal); TC_RUN(hex_oct_alt); TC_RUN(width_zero_pad); TC_RUN(precision_min_digits); TC_RUN(plus_space); TC_RUN(int_min); TC_RUN(hd_truncation_ld); return tc_end(); }
