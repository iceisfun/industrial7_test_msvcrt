/* TC
 * id:    pfm_1_star_width_integers
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (* field width, negative width = - flag); MS docs printf width specification
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(i) { char b[64]; sprintf(b, "[%*i]", 6, 42); CHECK_STR(b, "[    42]"); }
TC_CASE(o) { char b[64]; sprintf(b, "[%*o]", 6, 8); CHECK_STR(b, "[    10]"); }
TC_CASE(X) { char b[64]; sprintf(b, "[%*X]", 6, 255); CHECK_STR(b, "[    FF]"); }
TC_CASE(u) { char b[64]; sprintf(b, "[%*u]", 4, 7u); CHECK_STR(b, "[   7]"); }
TC_CASE(minus) { char b[64]; sprintf(b, "[%-*d]", 5, 3); CHECK_STR(b, "[3    ]"); }
TC_CASE(zero) { char b[64]; sprintf(b, "[%0*d]", 5, 3); CHECK_STR(b, "[00003]"); }
TC_CASE(negative_width) { char b[64]; sprintf(b, "[%*d]", -5, 3); CHECK_STR(b, "[3    ]"); }
int main(void) { tc_begin(); TC_RUN(i); TC_RUN(o); TC_RUN(X); TC_RUN(u); TC_RUN(minus); TC_RUN(zero); TC_RUN(negative_width); return tc_end(); }
