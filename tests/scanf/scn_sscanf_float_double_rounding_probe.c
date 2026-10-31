/* TC
 * id:    scn_sscanf_float_double_rounding_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   IEEE 754 correct rounding; C99 7.19.6.2 requires conversion as strtod
 * note:  PROBE: %f decimal-to-float conversion rounds once, directly to float (no double rounding).
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;

TC_CASE(int_2p24_plus1) { float f = 0; int r; v = "16777217"; r = sscanf((const char *)v, "%f", &f); CHECK_EQ_I(r, 1); CHECK_BITS_F(f, 16777216.0f); }
TC_CASE(just_below_tie) { float f = 0; int r; v = "1.00000017881393432617187499"; r = sscanf((const char *)v, "%f", &f); CHECK_EQ_I(r, 1); CHECK_EQ_U(tc_fbits(f), 0x3F800001u); }
TC_CASE(exact_tie_even) { float f = 0; int r; v = "1.000000178813934326171875"; r = sscanf((const char *)v, "%f", &f); CHECK_EQ_I(r, 1); CHECK_EQ_U(tc_fbits(f), 0x3F800002u); }
TC_CASE(tenth) { float f = 0; int r; v = "0.1"; r = sscanf((const char *)v, "%f", &f); CHECK_EQ_I(r, 1); CHECK_EQ_U(tc_fbits(f), 0x3DCCCCCDu); }
int main(void) { tc_begin(); TC_RUN(int_2p24_plus1); TC_RUN(just_below_tie); TC_RUN(exact_tie_even); TC_RUN(tenth); return tc_end(); }
