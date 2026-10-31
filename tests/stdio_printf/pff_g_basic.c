/* TC
 * id:    pff_g_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 g (P=6 default; exponent style only if X < -4 or X >= P)
 */
#include "tc.h"
#include <stdio.h>

static volatile double v;
TC_CASE(hundred_k) { char b[64]; v = 100000.0; sprintf(b, "%g", v); CHECK_STR(b, "100000"); }
TC_CASE(frac)      { char b[64]; v = 123.456; sprintf(b, "%g", v); CHECK_STR(b, "123.456"); }
TC_CASE(small_edge){ char b[64]; v = 0.0001; sprintf(b, "%g", v); CHECK_STR(b, "0.0001"); }
TC_CASE(one_half)  { char b[64]; v = 1.5; sprintf(b, "%g", v); CHECK_STR(b, "1.5"); }
TC_CASE(zero)      { char b[64]; v = 0.0; sprintf(b, "%g", v); CHECK_STR(b, "0"); }
TC_CASE(neg)       { char b[64]; v = -2.50; sprintf(b, "%g", v); CHECK_STR(b, "-2.5"); }
TC_CASE(neg_zero)  { char b[64]; v = -0.0; sprintf(b, "%g", v); CHECK_STR(b, "-0"); }

int main(void) { tc_begin(); TC_RUN(hundred_k); TC_RUN(frac); TC_RUN(small_edge); TC_RUN(one_half); TC_RUN(zero); TC_RUN(neg); TC_RUN(neg_zero); return tc_end(); }
