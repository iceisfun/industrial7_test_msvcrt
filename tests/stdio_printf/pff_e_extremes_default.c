/* TC
 * id:    pff_e_extremes_default
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 e; <float.h> DBL_MAX/DBL_MIN
 * note:  3-digit exponents occur naturally here, so the output is identical in every CRT.
 */
#include "tc.h"
#include <stdio.h>
#include <float.h>

static volatile double v;
TC_CASE(dbl_max)  { char b[64]; v = DBL_MAX; sprintf(b, "%e", v); CHECK_STR(b, "1.797693e+308"); }
TC_CASE(dbl_min)  { char b[64]; v = DBL_MIN; sprintf(b, "%e", v); CHECK_STR(b, "2.225074e-308"); }
TC_CASE(max_prec1) { char b[64]; v = DBL_MAX; sprintf(b, "%.1e", v); CHECK_STR(b, "1.8e+308"); }
TC_CASE(neg_max)  { char b[64]; v = -DBL_MAX; sprintf(b, "%e", v); CHECK_STR(b, "-1.797693e+308"); }

int main(void) { tc_begin(); TC_RUN(dbl_max); TC_RUN(dbl_min); TC_RUN(max_prec1); TC_RUN(neg_max); return tc_end(); }
