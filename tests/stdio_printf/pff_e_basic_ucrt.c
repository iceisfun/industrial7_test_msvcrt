/* TC
 * id:    pff_e_basic_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf format specification (e/E); C11 7.21.6.1
 * note:  UCRT prints at least two exponent digits (not three).
 */
#include "tc.h"
#include <stdio.h>

static volatile double v;
TC_CASE(basic)   { char b[64]; v = 1.5; sprintf(b, "%e", v); CHECK_STR(b, "1.500000e+00"); }
TC_CASE(zero)    { char b[64]; v = 0.0; sprintf(b, "%e", v); CHECK_STR(b, "0.000000e+00"); }
TC_CASE(prec2)   { char b[64]; v = 12345.678; sprintf(b, "%.2e", v); CHECK_STR(b, "1.23e+04"); }
TC_CASE(small)   { char b[64]; v = 1e-5; sprintf(b, "%e", v); CHECK_STR(b, "1.000000e-05"); }
TC_CASE(big)     { char b[64]; v = 1e100; sprintf(b, "%e", v); CHECK_STR(b, "1.000000e+100"); }
TC_CASE(upper)   { char b[64]; v = 1.5; sprintf(b, "%E", v); CHECK_STR(b, "1.500000E+00"); }
TC_CASE(prec0)   { char b[64]; v = 3.0; sprintf(b, "%.0e", v); CHECK_STR(b, "3e+00"); }
TC_CASE(neg)     { char b[64]; v = -0.00123; sprintf(b, "%.2e", v); CHECK_STR(b, "-1.23e-03"); }

int main(void) { tc_begin(); TC_RUN(basic); TC_RUN(zero); TC_RUN(prec2); TC_RUN(small); TC_RUN(big); TC_RUN(upper); TC_RUN(prec0); TC_RUN(neg); return tc_end(); }
