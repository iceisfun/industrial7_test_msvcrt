/* TC
 * id:    pff_wide_inf_nan_ucrt
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf format specification (UCRT prints inf, nan, -nan(ind)); C11 7.29.2.1
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define N(b) (sizeof(b) / sizeof((b)[0]))
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(inf)     { wchar_t b[32]; volatile double v = mk(0x7ff0000000000000ULL); int n = _snwprintf(b, N(b), L"%f", v); CHECK_WSTR(b, L"inf"); CHECK_EQ_I(n, 3); }
TC_CASE(neg_inf) { wchar_t b[32]; volatile double v = mk(0xfff0000000000000ULL); _snwprintf(b, N(b), L"%f", v); CHECK_WSTR(b, L"-inf"); }
TC_CASE(upper)   { wchar_t b[32]; volatile double v = mk(0x7ff0000000000000ULL); _snwprintf(b, N(b), L"%F", v); CHECK_WSTR(b, L"INF"); }
TC_CASE(qnan)    { wchar_t b[32]; volatile double v = mk(0x7ff8000000000000ULL); _snwprintf(b, N(b), L"%f", v); CHECK_WSTR(b, L"nan"); }
TC_CASE(indefinite) { wchar_t b[32]; volatile double v = mk(0xfff8000000000000ULL); int n = _snwprintf(b, N(b), L"%f", v); CHECK_WSTR(b, L"-nan(ind)"); CHECK_EQ_I(n, 9); }
int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(neg_inf); TC_RUN(upper); TC_RUN(qnan); TC_RUN(indefinite); return tc_end(); }
