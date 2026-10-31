/* TC
 * id:    pfs_str_s_width_left
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1p5-6; MS docs printf width specification
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile abc = "abc";
static volatile int w8 = 8, wn = -6, w5 = 5;
TC_CASE(right) { char b[64]; int n = sprintf(b, "[%8s]", abc); CHECK_STR(b, "[     abc]"); CHECK_EQ_I(n, 10); }
TC_CASE(left) { char b[64]; sprintf(b, "[%-8s]", abc); CHECK_STR(b, "[abc     ]"); }
TC_CASE(narrow_width) { char b[64]; sprintf(b, "[%2s]", abc); CHECK_STR(b, "[abc]"); }
TC_CASE(star) { char b[64]; sprintf(b, "[%*s]", w8, abc); CHECK_STR(b, "[     abc]"); }
TC_CASE(star_left) { char b[64]; sprintf(b, "[%-*s]", w5, abc); CHECK_STR(b, "[abc  ]"); }
TC_CASE(star_negative) { char b[64]; int n = sprintf(b, "[%*s]", wn, abc); CHECK_STR(b, "[abc   ]"); CHECK_EQ_I(n, 8); }
TC_CASE(ret) { char b[64]; int n = sprintf(b, "%8s|%-8s|", abc, abc); CHECK_EQ_I(n, 18); CHECK_EQ_U(strlen(b), 18); }
int main(void) { tc_begin(); TC_RUN(right); TC_RUN(left); TC_RUN(narrow_width); TC_RUN(star); TC_RUN(star_left); TC_RUN(star_negative); TC_RUN(ret); return tc_end(); }
