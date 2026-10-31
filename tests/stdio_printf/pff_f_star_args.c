/* TC
 * id:    pff_f_star_args
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 p4-5 (negative * width = - flag; negative * precision = omitted)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile double pi = 3.14159265358979;
static char b[64];

TC_CASE(both) { sprintf(b, "%*.*f", 10, 2, pi); CHECK_STR(b, "      3.14"); }
TC_CASE(neg_width) { sprintf(b, "[%*.2f]", -8, pi); CHECK_STR(b, "[3.14    ]"); }
TC_CASE(neg_prec) { sprintf(b, "%.*f", -1, pi); CHECK_STR(b, "3.141593"); }
TC_CASE(prec0) { sprintf(b, "%.*f", 0, pi); CHECK_STR(b, "3"); }
TC_CASE(minus) { sprintf(b, "[%-*.*f]", 9, 1, pi); CHECK_STR(b, "[3.1      ]"); }
int main(void) { tc_begin(); TC_RUN(both); TC_RUN(neg_width); TC_RUN(neg_prec); TC_RUN(prec0); TC_RUN(minus); return tc_end(); }
