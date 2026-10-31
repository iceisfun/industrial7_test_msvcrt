/* TC
 * id:    pfm_1_neg_snprintf_star_truncation
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf (returns -1 if the output would exceed count; no terminator when len == count)
 * note:  * width/precision producing more output than the buffer count.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(exact_fit_no_nul) {
    char b[8]; int i; volatile int wd = 4; int n;
    for (i = 0; i < 8; i++) b[i] = 'z';
    n = _snprintf(b, 4, "%*d", wd, 7);      /* exactly 4 chars: fits, no terminator */
    CHECK_EQ_I(n, 4); CHECK(b[0]==' ' && b[1]==' ' && b[2]==' ' && b[3]=='7'); CHECK(b[4]=='z');
}
TC_CASE(too_small) {
    char b[8]; int i; volatile int wd = 8; int n;
    for (i = 0; i < 8; i++) b[i] = 'z';
    n = _snprintf(b, 4, "%*d", wd, 7);      /* 8 chars > 4 */
    CHECK_EQ_I(n, -1);
    CHECK(b[0]==' ' && b[1]==' ' && b[2]==' ' && b[3]==' '); CHECK(b[4]=='z');
}
TC_CASE(star_prec_too_small) {
    char b[8]; int i; volatile int p = 6; int n;
    for (i = 0; i < 8; i++) b[i] = 'z';
    n = _snprintf(b, 3, "%.*d", p, 1);      /* 000001 > 3 */
    CHECK_EQ_I(n, -1); CHECK(b[0]=='0' && b[1]=='0' && b[2]=='0'); CHECK(b[3]=='z');
}
TC_CASE(wide_too_small) {
    wchar_t w[8]; int i; volatile int wd = 8; int n;
    for (i = 0; i < 8; i++) w[i] = L'z';
    n = _snwprintf(w, 4, L"%*d", wd, 7);
    CHECK_EQ_I(n, -1); CHECK(w[3]==L' '); CHECK(w[4]==L'z');
}

int main(void) { tc_begin(); TC_RUN(exact_fit_no_nul); TC_RUN(too_small); TC_RUN(star_prec_too_small); TC_RUN(wide_too_small); return tc_end(); }
