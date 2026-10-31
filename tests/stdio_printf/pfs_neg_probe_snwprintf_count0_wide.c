/* TC
 * id:    pfs_neg_probe_snwprintf_count0_wide
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snwprintf
 * note:  PROBE: _snwprintf(buf, 0, L"%%s", L"abcdef") with non-NULL buffer returns -1 (truncation) and writes nothing.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const wchar_t *volatile w6 = L"abcdef";

TC_CASE(zero_count) {
    wchar_t w[4]; int n; w[0] = L'Z';
    n = _snwprintf(w, 0, L"%s", w6);
    CHECK_EQ_I(n, -1); CHECK_EQ_U(w[0], 'Z');
}

int main(void) { tc_begin(); TC_RUN(zero_count); return tc_end(); }
