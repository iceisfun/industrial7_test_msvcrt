/* TC
 * id:    pff_f_float_promotion
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 6.5.2.2p6 default argument promotions; 7.21.6.1
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile float f01 = 0.1f, f24 = 16777216.0f, f15 = 1.5f;
static char b[96];

TC_CASE(f9) { sprintf(b, "%.9f", f01); CHECK_STR(b, "0.100000001"); }
TC_CASE(g17) { sprintf(b, "%.17g", f01); CHECK_STR(b, "0.10000000149011612"); }
TC_CASE(big) { sprintf(b, "%.0f", f24); CHECK_STR(b, "16777216"); }
TC_CASE(simple) { sprintf(b, "%f", f15); CHECK_STR(b, "1.500000"); }
int main(void) { tc_begin(); TC_RUN(f9); TC_RUN(g17); TC_RUN(big); TC_RUN(simple); return tc_end(); }
