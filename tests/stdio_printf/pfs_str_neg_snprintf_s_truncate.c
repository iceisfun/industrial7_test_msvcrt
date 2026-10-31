/* TC
 * id:    pfs_str_neg_snprintf_s_truncate
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: returns -1 if more than count chars; no NUL if count chars written
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile s6 = "abcdef";
TC_CASE(overflow_returns_neg) {
    char b[8]; memset(b, 0x7e, sizeof b);
    int n = _snprintf(b, 4, "%s", s6);
    CHECK_EQ_I(n, -1); CHECK_MEM(b, "abcd", 4); CHECK_EQ_U((unsigned char)b[4], 0x7e);
}
TC_CASE(exact_fit_no_nul) {
    char b[8]; memset(b, 0x7e, sizeof b);
    int n = _snprintf(b, 6, "%s", s6);
    CHECK_EQ_I(n, 6); CHECK_MEM(b, "abcdef", 6); CHECK_EQ_U((unsigned char)b[6], 0x7e);
}
int main(void) { tc_begin(); TC_RUN(overflow_returns_neg); TC_RUN(exact_fit_no_nul); return tc_end(); }
