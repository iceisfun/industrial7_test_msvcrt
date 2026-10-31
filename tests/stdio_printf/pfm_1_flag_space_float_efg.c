/* TC
 * id:    pfm_1_flag_space_float_efg
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.1; MS docs printf format specification (space flag; + overrides space)
 * note:  %F case only runs on ucrtbase. Exponent has 3 digits before UCRT.
 */
#include "tc.h"
#include <stdio.h>

#define EX(two, three) (TC_BELOW(140) ? (three) : (two))
static volatile double v1 = 1.0, vm1 = -1.0, vh = 0.5, vsm = 1e-10;

TC_CASE(space_e) { char b[64]; sprintf(b, "% e", v1); CHECK_STR(b, EX(" 1.000000e+00", " 1.000000e+000")); }
TC_CASE(space_f) { char b[64]; sprintf(b, "% f", v1); CHECK_STR(b, " 1.000000"); }
TC_CASE(space_g) { char b[64]; sprintf(b, "% g", vh); CHECK_STR(b, " 0.5"); }
TC_CASE(space_G) { char b[64]; sprintf(b, "% G", vsm); CHECK_STR(b, EX(" 1E-10", " 1E-010")); }
TC_CASE(space_negative) { char b[64]; sprintf(b, "% f", vm1); CHECK_STR(b, "-1.000000"); }
TC_CASE(plus_overrides) { char b[64]; sprintf(b, "%+ f", v1); CHECK_STR(b, "+1.000000"); sprintf(b, "% +f", v1); CHECK_STR(b, "+1.000000"); }
TC_CASE(space_F) {
#ifdef TC_UCRT
    char b[64]; sprintf(b, "% F", v1); CHECK_STR(b, " 1.000000");
#else
    TC_SKIP("%F is UCRT only here");
#endif
}
int main(void) { tc_begin(); TC_RUN(space_e); TC_RUN(space_f); TC_RUN(space_g); TC_RUN(space_G); TC_RUN(space_negative); TC_RUN(plus_overrides); TC_RUN(space_F); return tc_end(); }
