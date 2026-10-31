/* TC
 * id:    pff_f_hash_flag
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 p6 (# flag: decimal point always)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile double v5 = 5.0, v1 = 1.0, v2 = 2.0;
static char b[64];

TC_CASE(prec0) { int n = sprintf(b, "%#.0f", v5); CHECK_STR(b, "5."); CHECK_EQ_I(n, 2); sprintf(b, "%.0f", v5); CHECK_STR(b, "5"); }
TC_CASE(deflt) { sprintf(b, "%#f", v1); CHECK_STR(b, "1.000000"); }
TC_CASE(prec3) { sprintf(b, "%#.3f", v2); CHECK_STR(b, "2.000"); }
TC_CASE(width) { sprintf(b, "[%#10.0f][%-#6.0f]", v5, v5); CHECK_STR(b, "[        5.][5.    ]"); }
int main(void) { tc_begin(); TC_RUN(prec0); TC_RUN(deflt); TC_RUN(prec3); TC_RUN(width); return tc_end(); }
