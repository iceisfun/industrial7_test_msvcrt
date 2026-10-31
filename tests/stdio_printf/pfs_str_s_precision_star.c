/* TC
 * id:    pfs_str_s_precision_star
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1p4
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile s6 = "abcdef";
static volatile int two = 2, zero = 0, neg = -1, w8 = 8, w5 = 5, three = 3;
TC_CASE(star2) { char b[64]; sprintf(b, "%.*s", two, s6); CHECK_STR(b, "ab"); }
TC_CASE(star0) { char b[64]; sprintf(b, "[%.*s]", zero, s6); CHECK_STR(b, "[]"); }
TC_CASE(star_negative) { char b[64]; int n = sprintf(b, "%.*s", neg, s6); CHECK_STR(b, "abcdef"); CHECK_EQ_I(n, 6); }
TC_CASE(width_prec) { char b[64]; sprintf(b, "[%*.*s]", w8, three, s6); CHECK_STR(b, "[     abc]"); }
TC_CASE(left_width_prec) { char b[64]; sprintf(b, "[%-*.*s]", w5, two, s6); CHECK_STR(b, "[ab   ]"); }
TC_CASE(arg_order) { char b[64]; sprintf(b, "%.*s|%d", three, s6, 77); CHECK_STR(b, "abc|77"); }
int main(void) { tc_begin(); TC_RUN(star2); TC_RUN(star0); TC_RUN(star_negative); TC_RUN(width_prec); TC_RUN(left_width_prec); TC_RUN(arg_order); return tc_end(); }
