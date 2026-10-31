/* TC
 * id:    pfw_wide_inf_nan_ucrt
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.21.6.1 (inf / nan text); UCRT prints "inf", "nan", "-nan(ind)" for the x86/x64 default NaN
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static double dbl(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }

TC_CASE(f_inf) { wchar_t b[64]; volatile double v = dbl(0x7FF0000000000000ULL); _snwprintf(b, 64, L"%f|%e|%g", v, v, v); CHECK_WSTR(b, L"inf|inf|inf"); }
TC_CASE(f_neg_inf) { wchar_t b[64]; volatile double v = dbl(0xFFF0000000000000ULL); _snwprintf(b, 64, L"%f", v); CHECK_WSTR(b, L"-inf"); }
TC_CASE(f_nan) { wchar_t b[64]; volatile double v = dbl(0x7FF8000000000000ULL); _snwprintf(b, 64, L"%f|%g", v, v); CHECK_WSTR(b, L"nan|nan"); }
TC_CASE(upper) { wchar_t b[64]; volatile double v = dbl(0x7FF0000000000000ULL), n = dbl(0x7FF8000000000000ULL); _snwprintf(b, 64, L"%E|%G|%G", v, v, n); CHECK_WSTR(b, L"INF|INF|NAN"); }
TC_CASE(indefinite) { wchar_t b[64]; volatile double v = dbl(0xFFF8000000000000ULL); _snwprintf(b, 64, L"%f|%g", v, v); CHECK_WSTR(b, L"-nan(ind)|-nan(ind)"); }
TC_CASE(width_pad_inf) { wchar_t b[64]; volatile double v = dbl(0x7FF0000000000000ULL); _snwprintf(b, 64, L"[%6f][%-6f][%+f]", v, v, v); CHECK_WSTR(b, L"[   inf][inf   ][+inf]"); }

int main(void) { tc_begin(); TC_RUN(f_inf); TC_RUN(f_neg_inf); TC_RUN(f_nan); TC_RUN(upper); TC_RUN(indefinite); TC_RUN(width_pad_inf); return tc_end(); }
