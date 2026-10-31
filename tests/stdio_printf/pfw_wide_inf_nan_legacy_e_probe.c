/* TC
 * id:    pfw_wide_inf_nan_legacy_e_probe
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   <140
 * tier:  p2
 * conf:  probe
 * ref:   none (memory of real msvcr behaviour)
 * note:  PROBING: %e and %g of +inf on pre-UCRT CRTs. Expected "1.#INF00e+000" for %e and "1.#INF" for %g.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static double dbl(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }

TC_CASE(e_variant) { wchar_t b[64]; volatile double v = dbl(0x7FF0000000000000ULL); _snwprintf(b, 64, L"%e", v); CHECK_WSTR(b, L"1.#INF00e+000"); }
TC_CASE(g_variant) { wchar_t b[64]; volatile double v = dbl(0x7FF0000000000000ULL); _snwprintf(b, 64, L"%g", v); CHECK_WSTR(b, L"1.#INF"); }

int main(void) { tc_begin(); TC_RUN(e_variant); TC_RUN(g_variant); return tc_end(); }
