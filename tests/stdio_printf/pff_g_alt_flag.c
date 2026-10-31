/* TC
 * id:    pff_g_alt_flag
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (# with g: trailing zeros are not removed); MS docs printf flag directives
 */
#include "tc.h"
#include <stdio.h>

static volatile double v1_5 = 1.5, v100 = 100.0, v0 = 0.0, v1 = 1.0, v0_5 = 0.5;

TC_CASE(default_prec) { char b[64]; sprintf(b, "%#g", v1_5); CHECK_STR(b, "1.50000"); }
TC_CASE(point_kept) { char b[64]; sprintf(b, "%#.3g", v100); CHECK_STR(b, "100."); }
TC_CASE(zero) { char b[64]; sprintf(b, "%#g", v0); CHECK_STR(b, "0.00000"); }
TC_CASE(prec0) { char b[64]; sprintf(b, "%#.0g", v1); CHECK_STR(b, "1."); }
TC_CASE(small) { char b[64]; sprintf(b, "%#.3g", v0_5); CHECK_STR(b, "0.500"); }
TC_CASE(without_hash) { char b[64]; sprintf(b, "%g|%.3g", v1_5, v100); CHECK_STR(b, "1.5|100"); }

int main(void) { tc_begin(); TC_RUN(default_prec); TC_RUN(point_kept); TC_RUN(zero); TC_RUN(prec0); TC_RUN(small); TC_RUN(without_hash); return tc_end(); }
