/* TC
 * id:    pfi_int_star_width
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p5; MS docs printf width specification
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(basic) {
    char b[80]; volatile int w = 5;
    sprintf(b, "[%*d]", w, 42); CHECK_STR(b, "[   42]");
}
TC_CASE(zero_width) {
    char b[80]; volatile int w = 0;
    sprintf(b, "[%*d]", w, 42); CHECK_STR(b, "[42]");
}
TC_CASE(unsigned_hex) {
    char b[80]; volatile int w = 6, v = 255;
    sprintf(b, "[%*u][%*x]", w, 7u, w, v); CHECK_STR(b, "[     7][    ff]");
}
TC_CASE(smaller) {
    char b[80]; volatile int w = 2;
    sprintf(b, "[%*d]", w, 12345); CHECK_STR(b, "[12345]");
}
TC_CASE(zero_flag) {
    char b[80]; volatile int w = 6;
    sprintf(b, "[%0*d]", w, 42); CHECK_STR(b, "[000042]");
    sprintf(b, "[%0*d]", w, -42); CHECK_STR(b, "[-00042]");
}
TC_CASE(order) {
    char b[80]; volatile int w1 = 4, w2 = 6;
    sprintf(b, "[%*d][%*d]", w1, 1, w2, 2); CHECK_STR(b, "[   1][     2]");
    sprintf(b, "[%*d|%*d]", 3, 11, 5, 22); CHECK_STR(b, "[ 11|   22]");
}

int main(void) { tc_begin(); TC_RUN(basic); TC_RUN(zero_width); TC_RUN(unsigned_hex); TC_RUN(smaller); TC_RUN(zero_flag); TC_RUN(order); return tc_end(); }
