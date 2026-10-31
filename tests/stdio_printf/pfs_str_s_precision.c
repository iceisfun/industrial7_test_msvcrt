/* TC
 * id:    pfs_str_s_precision
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1p4; MS docs printf precision specification
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile s6 = "abcdef";
TC_CASE(prec3) { char b[64]; int n = sprintf(b, "%.3s", s6); CHECK_STR(b, "abc"); CHECK_EQ_I(n, 3); }
TC_CASE(prec0) { char b[64]; int n = sprintf(b, "[%.0s]", s6); CHECK_STR(b, "[]"); CHECK_EQ_I(n, 2); }
TC_CASE(prec_long) { char b[64]; sprintf(b, "%.10s", s6); CHECK_STR(b, "abcdef"); }
TC_CASE(width_prec) { char b[64]; sprintf(b, "[%8.3s]", s6); CHECK_STR(b, "[     abc]"); }
TC_CASE(left_prec) { char b[64]; sprintf(b, "[%-8.3s]", s6); CHECK_STR(b, "[abc     ]"); }
TC_CASE(period_only) { char b[64]; sprintf(b, "[%.s]", s6); CHECK_STR(b, "[]"); }
int main(void) { tc_begin(); TC_RUN(prec3); TC_RUN(prec0); TC_RUN(prec_long); TC_RUN(width_prec); TC_RUN(left_prec); TC_RUN(period_only); return tc_end(); }
