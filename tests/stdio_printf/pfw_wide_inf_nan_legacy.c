/* TC
 * id:    pfw_wide_inf_nan_legacy
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   <140
 * tier:  p1
 * conf:  observed
 * ref:   pre-UCRT CRTs print 1.#INF00 / 1.#QNAN00 / -1.#IND00 for %f
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static double dbl(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }

TC_CASE(inf) { wchar_t b[64]; volatile double v = dbl(0x7FF0000000000000ULL); _snwprintf(b, 64, L"%f", v); CHECK_WSTR(b, L"1.#INF00"); }
TC_CASE(neg_inf) { wchar_t b[64]; volatile double v = dbl(0xFFF0000000000000ULL); _snwprintf(b, 64, L"%f", v); CHECK_WSTR(b, L"-1.#INF00"); }
TC_CASE(qnan) { wchar_t b[64]; volatile double v = dbl(0x7FF8000000000000ULL); _snwprintf(b, 64, L"%f", v); CHECK_WSTR(b, L"1.#QNAN00"); }
TC_CASE(indefinite) { wchar_t b[64]; volatile double v = dbl(0xFFF8000000000000ULL); _snwprintf(b, 64, L"%f", v); CHECK_WSTR(b, L"-1.#IND00"); }

int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(neg_inf); TC_RUN(qnan); TC_RUN(indefinite); return tc_end(); }
