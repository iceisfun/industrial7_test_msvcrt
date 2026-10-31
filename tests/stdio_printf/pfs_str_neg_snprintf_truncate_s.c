/* TC
 * id:    pfs_str_neg_snprintf_truncate_s
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: count reached -> returns -1, no NUL appended
 * note:  kind negative: output truncated
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile s6 = "abcdef";
static const char *volatile s4 = "abcd";

TC_CASE(truncated) {
    char b[8]; int n, i; for (i = 0; i < 8; i++) b[i] = 'Z';
    n = _snprintf(b, 4, "%s", s6);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[3], 'd'); CHECK_EQ_I(b[4], 'Z');
}
TC_CASE(exact_fit_no_nul) {
    char b[8]; int n, i; for (i = 0; i < 8; i++) b[i] = 'Z';
    n = _snprintf(b, 4, "%s", s4);
    CHECK_EQ_I(n, 4); CHECK_EQ_I(b[3], 'd'); CHECK_EQ_I(b[4], 'Z');
}
TC_CASE(prec_makes_fit) {
    char b[8]; int n = _snprintf(b, 4, "%.3s", s6);
    CHECK_EQ_I(n, 3); CHECK_STR(b, "abc");
}

int main(void) { tc_begin(); TC_RUN(truncated); TC_RUN(exact_fit_no_nul); TC_RUN(prec_makes_fit); return tc_end(); }
