/* TC
 * id:    pfs_str_hs_hc_narrow
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf Size prefixes: h with c/s in narrow functions = single-byte
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile abc = "abc";
static volatile int X = 'x';
TC_CASE(hs) { char b[64]; int n = sprintf(b, "%hs", abc); CHECK_STR(b, "abc"); CHECK_EQ_I(n, 3); }
TC_CASE(hc) { char b[64]; int n = sprintf(b, "%hc", X); CHECK_STR(b, "x"); CHECK_EQ_I(n, 1); }
TC_CASE(width) { char b[64]; sprintf(b, "[%5hs][%-5hs]", abc, abc); CHECK_STR(b, "[  abc][abc  ]"); }
TC_CASE(prec) { char b[64]; sprintf(b, "%.2hs", abc); CHECK_STR(b, "ab"); }
int main(void) { tc_begin(); TC_RUN(hs); TC_RUN(hc); TC_RUN(width); TC_RUN(prec); return tc_end(); }
