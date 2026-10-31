/* TC
 * id:    pfm_1_flag_hash_float_efg
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (# flag); MS docs printf format specification
 * note:  %F case only runs on ucrtbase. Exponent has 3 digits before UCRT.
 */
#include "tc.h"
#include <stdio.h>

#define EX(two, three) (TC_BELOW(140) ? (three) : (two))
static volatile double v1 = 1.0, v3 = 3.0, v5 = 5.0, vsm = 1e-5, v1e5 = 100000.0;

TC_CASE(hash_e0) { char b[64]; sprintf(b, "%#.0e", v1); CHECK_STR(b, EX("1.e+00", "1.e+000")); }
TC_CASE(hash_f0) { char b[64]; sprintf(b, "%#.0f", v3); CHECK_STR(b, "3."); }
TC_CASE(hash_g) { char b[64]; sprintf(b, "%#g", v1); CHECK_STR(b, "1.00000"); }
TC_CASE(hash_G_exp) { char b[64]; sprintf(b, "%#G", vsm); CHECK_STR(b, EX("1.00000E-05", "1.00000E-005")); }
TC_CASE(hash_g_prec3) { char b[64]; sprintf(b, "%#.3g", v1); CHECK_STR(b, "1.00"); }
TC_CASE(hash_g_1e5) { char b[64]; sprintf(b, "%#g", v1e5); CHECK_STR(b, "100000."); }
TC_CASE(hash_F0) {
#ifdef TC_UCRT
    char b[64]; sprintf(b, "%#.0F", v5); CHECK_STR(b, "5.");
#else
    TC_SKIP("%F is UCRT only here");
#endif
}
int main(void) { tc_begin(); TC_RUN(hash_e0); TC_RUN(hash_f0); TC_RUN(hash_g); TC_RUN(hash_G_exp); TC_RUN(hash_g_prec3); TC_RUN(hash_g_1e5); TC_RUN(hash_F0); return tc_end(); }
