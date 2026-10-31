/* TC
 * id:    pff_wide_float_basic
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.1; MS docs _snwprintf
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define N(b) (sizeof(b) / sizeof((b)[0]))
TC_CASE(f_prec) { wchar_t b[32]; volatile double v = 1.5; int n = _snwprintf(b, N(b), L"%.3f", v); CHECK_WSTR(b, L"1.500"); CHECK_EQ_I(n, 5); }
TC_CASE(width)  { wchar_t b[32]; volatile double v = 3.14159, w = 2.5; int n = _snwprintf(b, N(b), L"%5.1f|%-6.2f|", v, w); CHECK_WSTR(b, L"  3.1|2.50  |"); CHECK_EQ_I(n, 13); }
TC_CASE(g)      { wchar_t b[32]; volatile double v = 100000.0, w = 0.5; int n = _snwprintf(b, N(b), L"%g|%g", v, w); CHECK_WSTR(b, L"100000|0.5"); CHECK_EQ_I(n, 10); }
TC_CASE(e)
{
    wchar_t b[32]; volatile double v = 1.5; int n = _snwprintf(b, N(b), L"%e", v);
#if TC_AT_LEAST(140)
    CHECK_WSTR(b, L"1.500000e+00"); CHECK_EQ_I(n, 12);
#else
    CHECK_WSTR(b, L"1.500000e+000"); CHECK_EQ_I(n, 13);
#endif
}
TC_CASE(neg_zero_flags) { wchar_t b[32]; volatile double v = 2.0; _snwprintf(b, N(b), L"%+.1f|% .1f|%08.2f", v, v, -v); CHECK_WSTR(b, L"+2.0| 2.0|-0002.00"); }
int main(void) { tc_begin(); TC_RUN(f_prec); TC_RUN(width); TC_RUN(g); TC_RUN(e); TC_RUN(neg_zero_flags); return tc_end(); }
