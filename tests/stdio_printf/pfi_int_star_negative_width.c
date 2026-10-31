/* TC
 * id:    pfi_int_star_negative_width
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p5 (negative field width = - flag and positive width)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(d) {
    char b[80]; volatile int w = -5;
    sprintf(b, "[%*d]", w, 42); CHECK_STR(b, "[42   ]");
}
TC_CASE(u_x) {
    char b[80]; volatile int w3 = -3, w4 = -4;
    sprintf(b, "[%*u]", w3, 7u); CHECK_STR(b, "[7  ]");
    sprintf(b, "[%*x]", w4, 255); CHECK_STR(b, "[ff  ]");
}
TC_CASE(minus_flag_too) {
    char b[80]; volatile int w = -5;
    sprintf(b, "[%-*d]", w, 42); CHECK_STR(b, "[42   ]");
}
TC_CASE(zero_overridden) {
    char b[80]; volatile int w = -5;
    sprintf(b, "[%0*d]", w, 42); CHECK_STR(b, "[42   ]");
}

int main(void) { tc_begin(); TC_RUN(d); TC_RUN(u_x); TC_RUN(minus_flag_too); TC_RUN(zero_overridden); return tc_end(); }
