/* TC
 * id:    pff_neg_wide_snwprintf_float_trunc
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snwprintf (returns negative if more than count wide chars needed; count chars still written)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
TC_CASE(too_long) { wchar_t b[16]; volatile double v = 1.5; int n; fill(b); n = _snwprintf(b, 4, L"%f", v);
    CHECK(n < 0); CHECK(b[0] == L'1' && b[1] == L'.' && b[2] == L'5' && b[3] == L'0'); CHECK(b[4] == L'Z'); }
TC_CASE(exact) { wchar_t b[16]; volatile double v = 1.5; int n; fill(b); n = _snwprintf(b, 8, L"%f", v);
    CHECK_EQ_I(n, 8); CHECK(b[7] == L'0'); CHECK(b[8] == L'Z'); }
TC_CASE(fits) { wchar_t b[16]; volatile double v = 1.5; int n; fill(b); n = _snwprintf(b, 9, L"%f", v); CHECK_EQ_I(n, 8); CHECK_WSTR(b, L"1.500000"); }
int main(void) { tc_begin(); TC_RUN(too_long); TC_RUN(exact); TC_RUN(fits); return tc_end(); }
