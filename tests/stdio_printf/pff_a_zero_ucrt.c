/* TC
 * id:    pff_a_zero_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1 a/A (leading digit of a zero value is unspecified)
 * note:  PROBING: C leaves the leading digit of zero unspecified; UCRT is expected to print 0x0.000p+0. Zero is printed with a single 0 digit before the point, with the given number of fraction digits.
 */
#include "tc.h"
#include <stdio.h>

static volatile double z = 0.0, nz = -0.0;

TC_CASE(zero_p3) { char b[64]; sprintf(b, "%.3a", z); CHECK_STR(b, "0x0.000p+0"); }
TC_CASE(negzero_p3) { char b[64]; sprintf(b, "%.3a", nz); CHECK_STR(b, "-0x0.000p+0"); }
TC_CASE(zero_p0) { char b[64]; sprintf(b, "%.0a", z); CHECK_STR(b, "0x0p+0"); }
TC_CASE(zero_upper) { char b[64]; sprintf(b, "%.1A", z); CHECK_STR(b, "0X0.0P+0"); }

int main(void) { tc_begin(); TC_RUN(zero_p3); TC_RUN(negzero_p3); TC_RUN(zero_p0); TC_RUN(zero_upper); return tc_end(); }
