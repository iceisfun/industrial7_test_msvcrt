/* TC
 * id:    pff_g_trailing_zeros
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 g: trailing zeros removed unless # flag; no decimal point if no fraction remains
 */
#include "tc.h"
#include <stdio.h>

static volatile double v;
TC_CASE(one_half)  { char b[64]; v = 1.5; sprintf(b, "%g", v); CHECK_STR(b, "1.5"); }
TC_CASE(hundred)   { char b[64]; v = 100.0; sprintf(b, "%g", v); CHECK_STR(b, "100"); }
TC_CASE(tenth)     { char b[64]; v = 0.1; sprintf(b, "%.10g", v); CHECK_STR(b, "0.1"); }
TC_CASE(one_p3)    { char b[64]; v = 1.0; sprintf(b, "%.3g", v); CHECK_STR(b, "1"); }
TC_CASE(two_half)  { char b[64]; v = 2.50000; sprintf(b, "%.6g", v); CHECK_STR(b, "2.5"); }
TC_CASE(hundred_k) { char b[64]; v = 1e5; sprintf(b, "%g", v); CHECK_STR(b, "100000"); }
TC_CASE(alt_keeps) { char b[64]; v = 1.0; sprintf(b, "%#.3g", v); CHECK_STR(b, "1.00"); v = 1.5; sprintf(b, "%#g", v); CHECK_STR(b, "1.50000"); }

int main(void) { tc_begin(); TC_RUN(one_half); TC_RUN(hundred); TC_RUN(tenth); TC_RUN(one_p3); TC_RUN(two_half); TC_RUN(hundred_k); TC_RUN(alt_keeps); return tc_end(); }
