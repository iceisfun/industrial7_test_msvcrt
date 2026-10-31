/* TC
 * id:    pfm_1_flag_plus_float_efg
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.1; MS docs printf format specification (+ flag; exponent has 3 digits before UCRT, 2 in UCRT)
 * note:  %F cases only run on ucrtbase.
 */
#include "tc.h"
#include <stdio.h>

#define EX(two, three) (TC_BELOW(140) ? (three) : (two))
static volatile double v15 = 1.5, vz = 0.0, vnz = -0.0, v1e5 = 100000.0, v1e6 = 1e6, vsm = 1e-5, v25 = 2.5;

TC_CASE(plus_e) { char b[64]; sprintf(b, "%+e", v15); CHECK_STR(b, EX("+1.500000e+00", "+1.500000e+000")); }
TC_CASE(plus_E_zero) { char b[64]; sprintf(b, "%+E", vz); CHECK_STR(b, EX("+0.000000E+00", "+0.000000E+000")); }
TC_CASE(plus_g) { char b[64]; sprintf(b, "%+g", v1e5); CHECK_STR(b, "+100000"); sprintf(b, "%+g", v1e6); CHECK_STR(b, EX("+1e+06", "+1e+006")); }
TC_CASE(plus_G) { char b[64]; sprintf(b, "%+G", vsm); CHECK_STR(b, EX("+1E-05", "+1E-005")); }
TC_CASE(plus_f_zeros) { char b[64]; sprintf(b, "%+f", vnz); CHECK_STR(b, "-0.000000"); sprintf(b, "%+f", vz); CHECK_STR(b, "+0.000000"); }
TC_CASE(plus_F) {
#ifdef TC_UCRT
    char b[64]; sprintf(b, "%+F", v25); CHECK_STR(b, "+2.500000");
#else
    TC_SKIP("%F is UCRT only here");
#endif
}
int main(void) { tc_begin(); TC_RUN(plus_e); TC_RUN(plus_E_zero); TC_RUN(plus_g); TC_RUN(plus_G); TC_RUN(plus_f_zeros); TC_RUN(plus_F); return tc_end(); }
