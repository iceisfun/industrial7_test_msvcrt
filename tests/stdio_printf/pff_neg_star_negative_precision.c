/* TC
 * id:    pff_neg_star_negative_precision
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 p4-5 (a negative precision argument is taken as if the precision were omitted; negative width = '-' flag)
 * note:  Negative values supplied through * must not be treated as errors. The INT_MIN precision case is in pff_neg_star_precision_int_min (split so a crash there cannot mask these).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(prec_minus1_f)   { char b[64]; volatile int p = -1; int r = sprintf(b, "%.*f", p, 2.25); CHECK_STR(b, "2.250000"); CHECK_EQ_I(r, 8); }
TC_CASE(prec_minus1_g)   { char b[64]; volatile int p = -7; sprintf(b, "%.*g", p, 0.5); CHECK_STR(b, "0.5"); }
TC_CASE(width_minus5_left) { char b[64]; volatile int w = -5; int r = sprintf(b, "[%*.1f]", w, 1.5); CHECK_STR(b, "[1.5  ]"); CHECK_EQ_I(r, 7); }
TC_CASE(width_minus5_zero_flag) { char b[64]; volatile int w = -6; sprintf(b, "[%0*.1f]", w, 1.5); CHECK_STR(b, "[1.5   ]"); }

int main(void) { tc_begin(); TC_RUN(prec_minus1_f); TC_RUN(prec_minus1_g); TC_RUN(width_minus5_left); TC_RUN(width_minus5_zero_flag); return tc_end(); }
