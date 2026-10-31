/* TC
 * id:    pfs_str_percent_literal
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 p8 (%%)

 */
#include "tc.h"
#include <stdio.h>

static const char *volatile s = "ab";
static volatile int ch = 'z';

TC_CASE(pct) { char b[16]; int n = sprintf(b, "%%"); CHECK_STR(b, "%"); CHECK_EQ_I(n, 1); }
TC_CASE(pct_s) { char b[16]; int n = sprintf(b, "%%s"); CHECK_STR(b, "%s"); CHECK_EQ_I(n, 2); }
TC_CASE(s_pct) { char b[16]; int n = sprintf(b, "%s%% ", s); CHECK_STR(b, "ab% "); CHECK_EQ_I(n, 4); }
TC_CASE(hundred) { char b[16]; sprintf(b, "100%% %s", s); CHECK_STR(b, "100% ab"); }
TC_CASE(c_pct) { char b[16]; int n = sprintf(b, "%c%%", ch); CHECK_STR(b, "z%"); CHECK_EQ_I(n, 2); }

int main(void) { tc_begin(); TC_RUN(pct); TC_RUN(pct_s); TC_RUN(s_pct); TC_RUN(hundred); TC_RUN(c_pct); return tc_end(); }
