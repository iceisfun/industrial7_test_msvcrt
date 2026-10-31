/* TC
 * id:    pff_e_basic_legacy
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf (pre-VS2015): exponent has three digits
 * note:  Pre-UCRT CRTs print three exponent digits by default.
 */
#include "tc.h"
#include <stdio.h>

static volatile double v;
TC_CASE(basic)   { char b[64]; v = 1.5; sprintf(b, "%e", v); CHECK_STR(b, "1.500000e+000"); }
TC_CASE(zero)    { char b[64]; v = 0.0; sprintf(b, "%e", v); CHECK_STR(b, "0.000000e+000"); }
TC_CASE(prec2)   { char b[64]; v = 12345.678; sprintf(b, "%.2e", v); CHECK_STR(b, "1.23e+004"); }
TC_CASE(small)   { char b[64]; v = 1e-5; sprintf(b, "%e", v); CHECK_STR(b, "1.000000e-005"); }
TC_CASE(big)     { char b[64]; v = 1e100; sprintf(b, "%e", v); CHECK_STR(b, "1.000000e+100"); }
TC_CASE(upper)   { char b[64]; v = 1.5; sprintf(b, "%E", v); CHECK_STR(b, "1.500000E+000"); }

int main(void) { tc_begin(); TC_RUN(basic); TC_RUN(zero); TC_RUN(prec2); TC_RUN(small); TC_RUN(big); TC_RUN(upper); return tc_end(); }
