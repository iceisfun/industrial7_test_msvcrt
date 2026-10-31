/* TC
 * id:    pfs_wstr_c_nul
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.29.2.1 (c conversion writes the character, including NUL); MS docs

 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile int zero = 0;

TC_CASE(c_zero) {
    wchar_t w[8]; int n, i; for (i = 0; i < 8; i++) w[i] = 0x7777;
    n = _snwprintf(w, 8, L"a%cb", zero);
    CHECK_EQ_I(n, 3); CHECK_EQ_U(w[0], 'a'); CHECK_EQ_U(w[1], 0); CHECK_EQ_U(w[2], 'b');
}

int main(void) { tc_begin(); TC_RUN(c_zero); return tc_end(); }
