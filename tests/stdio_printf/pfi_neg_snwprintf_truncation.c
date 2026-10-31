/* TC
 * id:    pfi_neg_snwprintf_truncation
 * area:  stdio_printf
 * funcs: _snwprintf
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf: -1 if more than count chars; no NUL when exactly count
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
static void fillw(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }

TC_CASE(overflow) {
    wchar_t b[16]; int r; fillw(b);
    r = _snwprintf(b, 3, L"%d", 12345);
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == L'1' && b[1] == L'2' && b[2] == L'3');
    CHECK(b[3] == L'Z');
}
TC_CASE(exact_no_nul) {
    wchar_t b[16]; int r; fillw(b);
    r = _snwprintf(b, 5, L"%d", 12345);
    CHECK_EQ_I(r, 5);
    CHECK(b[4] == L'5'); CHECK(b[5] == L'Z');
}
int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(exact_no_nul); return tc_end(); }
