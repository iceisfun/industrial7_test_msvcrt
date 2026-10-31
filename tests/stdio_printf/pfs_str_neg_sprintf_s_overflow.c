/* TC
 * id:    pfs_str_neg_sprintf_s_overflow
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s: buffer too small -> buffer[0]=0 and invalid parameter handler invoked; returns -1 on error
 * note:  sprintf_s with %s; the string does not fit. Buffer size 4. errno is deliberately not asserted here (not documented); see pfs_neg_probe_sprintf_s_overflow_errno.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#include <errno.h>
#undef sprintf_s
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define sprintf_s tc_p_sprintf_s
static const char *volatile s6 = "abcdef";
TC_CASE(overflow) {
    char b[4]; int n; memset(b, 0x7e, sizeof b);
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    n = sprintf_s(b, sizeof b, "%s", s6);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(fits_exactly) {
    char b[7]; int n; tc_iph_count = 0;
    n = sprintf_s(b, sizeof b, "%s", s6);
    CHECK_EQ_I(n, 6); CHECK_STR(b, "abcdef"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(one_too_small) {
    char b[6]; int n; tc_iph_count = 0; errno = 0;
    n = sprintf_s(b, sizeof b, "%s", s6);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_LOAD_PROC(sprintf_s); if (!tc_p_sprintf_s) { tc_write("TC FAIL no sprintf_s\n"); return 1; } TC_RUN(overflow); TC_RUN(fits_exactly); TC_RUN(one_too_small); return tc_end(); }
