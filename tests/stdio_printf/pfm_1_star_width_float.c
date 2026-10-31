/* TC
 * id:    pfm_1_star_width_float
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (* width); MS docs printf width specification
 * note:  %F, %a, %A cases only run on ucrtbase (explicit precision for a/A). Exponent has 3 digits before UCRT.
 */
#include "tc.h"
#include <stdio.h>

#define EX(two, three) (TC_BELOW(140) ? (three) : (two))
static volatile double v15 = 1.5, v1 = 1.0, vsm = 1e-5;

TC_CASE(star_e) { char b[64]; sprintf(b, "%*e", 14, v15); CHECK_STR(b, EX("  1.500000e+00", " 1.500000e+000")); }
TC_CASE(star_E) { char b[64]; sprintf(b, "%*E", 14, v15); CHECK_STR(b, EX("  1.500000E+00", " 1.500000E+000")); }
TC_CASE(star_g) { char b[64]; sprintf(b, "%*g", 8, v15); CHECK_STR(b, "     1.5"); }
TC_CASE(star_G) { char b[64]; sprintf(b, "%*G", 8, vsm); CHECK_STR(b, EX("   1E-05", "  1E-005")); }
TC_CASE(star_F) {
#ifdef TC_UCRT
    char b[64]; sprintf(b, "%*F", 10, v15); CHECK_STR(b, "  1.500000");
#else
    TC_SKIP("%F is UCRT only here");
#endif
}
TC_CASE(star_a) {
#ifdef TC_UCRT
    char b[64]; sprintf(b, "%*.1a", 12, v1); CHECK_STR(b, "    0x1.0p+0");
#else
    TC_SKIP("%a is UCRT only here");
#endif
}
TC_CASE(star_A) {
#ifdef TC_UCRT
    char b[64]; sprintf(b, "%*.1A", 12, v1); CHECK_STR(b, "    0X1.0P+0");
#else
    TC_SKIP("%A is UCRT only here");
#endif
}
int main(void) { tc_begin(); TC_RUN(star_e); TC_RUN(star_E); TC_RUN(star_g); TC_RUN(star_G); TC_RUN(star_F); TC_RUN(star_a); TC_RUN(star_A); return tc_end(); }
