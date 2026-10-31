/* TC
 * id:    pff_f_cap_f_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C99 7.19.6.1 F conversion; MS docs printf
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile double v = 1.5, v2 = 2.0, v10 = 1e10, vs = 1.5e-5;
static char b[96];

TC_CASE(F) { sprintf(b, "%F", v); CHECK_STR(b, "1.500000"); }
TC_CASE(F2) { sprintf(b, "%.2F", v2); CHECK_STR(b, "2.00"); }
TC_CASE(F1e10) { sprintf(b, "%F", v10); CHECK_STR(b, "10000000000.000000"); }
TC_CASE(upper) { sprintf(b, "%E|%G", vs, vs); CHECK_STR(b, "1.500000E-05|1.5E-05"); }
int main(void) { tc_begin(); TC_RUN(F); TC_RUN(F2); TC_RUN(F1e10); TC_RUN(upper); return tc_end(); }
