/* TC
 * id:    pfm_1_flag_minus_float_efg_caps
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (- flag, width); MS docs printf format specification
 * note:  %F case only runs on ucrtbase. Exponent has 3 digits before UCRT.
 */
#include "tc.h"
#include <stdio.h>

#define EX(two, three) (TC_BELOW(140) ? (three) : (two))
static volatile double v15 = 1.5, vm15 = -1.5;

TC_CASE(minus_E) { char b[64]; sprintf(b, "%-14E", v15); CHECK_STR(b, EX("1.500000E+00  ", "1.500000E+000 ")); }
TC_CASE(minus_G) { char b[64]; sprintf(b, "%-8G", v15); CHECK_STR(b, "1.5     "); }
TC_CASE(minus_e_neg) { char b[64]; sprintf(b, "%-14e", vm15); CHECK_STR(b, EX("-1.500000e+00 ", "-1.500000e+000")); }
TC_CASE(narrow_width) { char b[64]; int n = sprintf(b, "%-3f", v15); CHECK_STR(b, "1.500000"); CHECK_EQ_I(n, 8); }
TC_CASE(minus_F) {
#ifdef TC_UCRT
    char b[64]; sprintf(b, "%-10F", v15); CHECK_STR(b, "1.500000  ");
#else
    TC_SKIP("%F is UCRT only here");
#endif
}
int main(void) { tc_begin(); TC_RUN(minus_E); TC_RUN(minus_G); TC_RUN(minus_e_neg); TC_RUN(narrow_width); TC_RUN(minus_F); return tc_end(); }
