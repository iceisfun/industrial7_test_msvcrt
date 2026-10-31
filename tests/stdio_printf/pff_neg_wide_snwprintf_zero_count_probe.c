/* TC
 * id:    pff_neg_wide_snwprintf_zero_count_probe
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snwprintf (returns negative if more than count wide chars needed; count chars still written)
 * note:  PROBE: _snwprintf(buf, 0, ...) with non-empty output returns a negative value and writes nothing.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
TC_CASE(zero_count) { wchar_t b[16]; volatile double v = 1.5; int n; fill(b); n = _snwprintf(b, 0, L"%f", v); CHECK(n < 0); CHECK(b[0] == L'Z'); }
int main(void) { tc_begin(); TC_RUN(zero_count); return tc_end(); }
