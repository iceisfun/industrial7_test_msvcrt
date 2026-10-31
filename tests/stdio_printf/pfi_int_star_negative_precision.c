/* TC
 * id:    pfi_int_star_negative_precision
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p5 (negative precision taken as if omitted)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(d) {
    char b[80]; volatile int p = -1;
    sprintf(b, "[%.*d]", p, 42); CHECK_STR(b, "[42]");
}
TC_CASE(zero_not_empty) {
    char b[80]; volatile int p = -5;
    sprintf(b, "[%.*d]", p, 0); CHECK_STR(b, "[0]");
}
TC_CASE(zero_flag_effective) {
    char b[80]; volatile int p = -1;
    sprintf(b, "[%08.*d]", p, 42); CHECK_STR(b, "[00000042]");
}
TC_CASE(hex) {
    char b[80]; volatile int p = -2;
    sprintf(b, "[%.*x]", p, 255); CHECK_STR(b, "[ff]");
}

int main(void) { tc_begin(); TC_RUN(d); TC_RUN(zero_not_empty); TC_RUN(zero_flag_effective); TC_RUN(hex); return tc_end(); }
