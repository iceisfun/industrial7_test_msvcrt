/* TC
 * id:    pff_wide_snan_ucrt_probe
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf format specification (nan(snan))
 * note:  PROBE: wide %f of a signalling NaN (0x7ff0000000000001) is "nan(snan)", negative one "-nan(snan)". Also %e/%g share the spelling.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define N(b) (sizeof(b) / sizeof((b)[0]))
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(snan)     { wchar_t b[32]; volatile double v = mk(0x7ff0000000000001ULL); _snwprintf(b, N(b), L"%f", v); CHECK_WSTR(b, L"nan(snan)"); }
TC_CASE(neg_snan) { wchar_t b[32]; volatile double v = mk(0xfff0000000000001ULL); _snwprintf(b, N(b), L"%f", v); CHECK_WSTR(b, L"-nan(snan)"); }
TC_CASE(snan_g)   { wchar_t b[32]; volatile double v = mk(0x7ff0000000000001ULL); _snwprintf(b, N(b), L"%g", v); CHECK_WSTR(b, L"nan(snan)"); }
int main(void) { tc_begin(); TC_RUN(snan); TC_RUN(neg_snan); TC_RUN(snan_g); return tc_end(); }
