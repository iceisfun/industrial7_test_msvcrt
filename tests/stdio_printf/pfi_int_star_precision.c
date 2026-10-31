/* TC
 * id:    pfi_int_star_precision
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p5; MS docs printf precision specification
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(basic) {
    char b[80]; volatile int p = 5;
    sprintf(b, "%.*d", p, 42); CHECK_STR(b, "00042");
}
TC_CASE(zero_zero) {
    char b[80]; volatile int p = 0; int n;
    n = sprintf(b, "%.*d", p, 0); CHECK_STR(b, ""); CHECK_EQ_I(n, 0);
    sprintf(b, "%.*d", p, 5); CHECK_STR(b, "5");
}
TC_CASE(hex) {
    char b[80]; volatile int p = 4;
    sprintf(b, "%.*x", p, 0xab); CHECK_STR(b, "00ab");
    sprintf(b, "%.*X", p, 0xab); CHECK_STR(b, "00AB");
}
TC_CASE(width_and_prec) {
    char b[80]; volatile int w = 8, p = 4;
    sprintf(b, "[%*.*d]", w, p, 42); CHECK_STR(b, "[    0042]");
    sprintf(b, "[%-*.*d]", w, p, 42); CHECK_STR(b, "[0042    ]");
}
TC_CASE(octal_alt) {
    char b[80]; volatile int p = 4, z = 0;
    sprintf(b, "%#.*o", p, 8); CHECK_STR(b, "0010");
    sprintf(b, "%#.*o", z, 8); CHECK_STR(b, "010");
}

int main(void) { tc_begin(); TC_RUN(basic); TC_RUN(zero_zero); TC_RUN(hex); TC_RUN(width_and_prec); TC_RUN(octal_alt); return tc_end(); }
