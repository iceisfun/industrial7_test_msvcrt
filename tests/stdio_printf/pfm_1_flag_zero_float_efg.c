/* TC
 * id:    pfm_1_flag_zero_float_efg
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (0 flag; ignored when - present); MS docs printf format specification
 * note:  %F case only runs on ucrtbase. Exponent has 3 digits before UCRT, widths chosen so padding is visible.
 */
#include "tc.h"
#include <stdio.h>

#define EX(two, three) (TC_BELOW(140) ? (three) : (two))
static volatile double v15 = 1.5, vm15 = -1.5, v2 = 2.0, vsm = 1.5e-5;

TC_CASE(zero_e) { char b[64]; sprintf(b, "%014e", v15); CHECK_STR(b, EX("001.500000e+00", "01.500000e+000")); }
TC_CASE(zero_f_neg) { char b[64]; sprintf(b, "%010.2f", vm15); CHECK_STR(b, "-000001.50"); }
TC_CASE(zero_g) { char b[64]; sprintf(b, "%010g", v15); CHECK_STR(b, "00000001.5"); }
TC_CASE(zero_G) { char b[64]; sprintf(b, "%010G", vsm); CHECK_STR(b, EX("0001.5E-05", "001.5E-005")); }
TC_CASE(zero_plus) { char b[64]; sprintf(b, "%+010.1f", v2); CHECK_STR(b, "+0000002.0"); }
TC_CASE(minus_overrides_zero) { char b[64]; sprintf(b, "%-010.1f", v2); CHECK_STR(b, "2.0       "); }
TC_CASE(zero_F) {
#ifdef TC_UCRT
    char b[64]; sprintf(b, "%010F", v15); CHECK_STR(b, "001.500000");
#else
    TC_SKIP("%F is UCRT only here");
#endif
}
int main(void) { tc_begin(); TC_RUN(zero_e); TC_RUN(zero_f_neg); TC_RUN(zero_g); TC_RUN(zero_G); TC_RUN(zero_plus); TC_RUN(minus_overrides_zero); TC_RUN(zero_F); return tc_end(); }
