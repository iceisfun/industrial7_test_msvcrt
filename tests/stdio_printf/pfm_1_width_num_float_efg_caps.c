/* TC
 * id:    pfm_1_width_num_float_efg_caps
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (field width, F, a/A); MS docs printf format specification
 * note:  %F, %a, %A cases only run on ucrtbase (explicit precision avoids default-precision differences). Exponent has 3 digits before UCRT.
 */
#include "tc.h"
#include <stdio.h>

#define EX(two, three) (TC_BELOW(140) ? (three) : (two))
static volatile double v15 = 1.5, v1 = 1.0, vsm = 0.00001, v1e20 = 1e20;

TC_CASE(width_f) { char b[64]; sprintf(b, "%12f", v15); CHECK_STR(b, "    1.500000"); }
TC_CASE(width_G) { char b[64]; sprintf(b, "%8G", vsm); CHECK_STR(b, EX("   1E-05", "  1E-005")); }
TC_CASE(width_g) { char b[64]; sprintf(b, "%10g", v1e20); CHECK_STR(b, EX("     1e+20", "    1e+020")); }
TC_CASE(width_F) {
#ifdef TC_UCRT
    char b[64]; sprintf(b, "%12F", v15); CHECK_STR(b, "    1.500000");
#else
    TC_SKIP("%F is UCRT only here");
#endif
}
TC_CASE(width_a) {
#ifdef TC_UCRT
    char b[64]; sprintf(b, "%14.1a", v1); CHECK_STR(b, "      0x1.0p+0");
#else
    TC_SKIP("%a is UCRT only here");
#endif
}
TC_CASE(width_A) {
#ifdef TC_UCRT
    char b[64]; sprintf(b, "%12.1A", v1); CHECK_STR(b, "    0X1.0P+0");
#else
    TC_SKIP("%A is UCRT only here");
#endif
}
int main(void) { tc_begin(); TC_RUN(width_f); TC_RUN(width_G); TC_RUN(width_g); TC_RUN(width_F); TC_RUN(width_a); TC_RUN(width_A); return tc_end(); }
