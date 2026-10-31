/* TC
 * id:    pfm_1_neg_sprintf_s_star_overflow
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s (buffer too small: invalid parameter, returns -1, errno ERANGE, buffer[0] = 0)
 * note:  * precision/width making output exceed the sprintf_s buffer.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(width_overflow) {
    char b[8]; int r; volatile int wd = 20;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    b[0] = 'q'; tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, sizeof b, "%*d", wd, 5);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
}
TC_CASE(prec_overflow) {
    char b[8]; int r; volatile int p = 12;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    b[0] = 'q'; tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, sizeof b, "%.*d", p, 5);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
}
TC_CASE(fits) {
    char b[8]; int r; volatile int p = 3;
    tc_iph_install(); tc_iph_count = 0;
    r = sprintf_s(b, sizeof b, "%.*d", p, 5);
    CHECK_EQ_I(r, 3); CHECK_STR(b, "005"); CHECK_EQ_I(tc_iph_count, 0);
}

int main(void) { tc_begin(); TC_RUN(width_overflow); TC_RUN(prec_overflow); TC_RUN(fits); return tc_end(); }
