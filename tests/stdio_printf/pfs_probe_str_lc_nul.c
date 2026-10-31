/* TC
 * id:    pfs_probe_str_lc_nul
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.1 (c)
 * note:  PROBE: %lc of wchar_t 0 in narrow sprintf converts to a single NUL byte and counts as 1 char (wctomb of L'\\0' returns 1).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile wchar_t wz = 0;

TC_CASE(nul) {
    char b[8]; int n;
    b[0] = 'x'; b[1] = 'y'; b[2] = 'z'; b[3] = 0;
    n = sprintf(b, "a%lcb", wz);
    CHECK_EQ_I(n, 3); CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], 'b');
}

int main(void) { tc_begin(); TC_RUN(nul); return tc_end(); }
