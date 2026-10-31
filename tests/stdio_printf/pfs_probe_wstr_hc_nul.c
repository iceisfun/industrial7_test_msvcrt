/* TC
 * id:    pfs_probe_wstr_hc_nul
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf type field characters
 * note:  PROBE: %hc of a NUL narrow char in wide output yields one L'\\0' (mbtowc of NUL returns 0, not an error); count 1.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile int zero = 0;

TC_CASE(hc_zero) {
    wchar_t w[8]; int n, i; for (i = 0; i < 8; i++) w[i] = 0x7777;
    n = _snwprintf(w, 8, L"a%hcb", zero);
    CHECK_EQ_I(n, 3); CHECK_EQ_U(w[0], 'a'); CHECK_EQ_U(w[1], 0); CHECK_EQ_U(w[2], 'b');
}

int main(void) { tc_begin(); TC_RUN(hc_zero); return tc_end(); }
