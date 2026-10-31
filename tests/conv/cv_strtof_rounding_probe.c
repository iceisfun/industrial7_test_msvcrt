/* TC
 * id:    cv_strtof_rounding_probe
 * area:  conv
 * funcs: strtof
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.22.1.3 / F.5 (decimal->binary conversion should round correctly)
 * note:  PROBE: is strtof correctly rounded straight to float (no double rounding)?
 *        1.00000017881393432617187499 is just BELOW the midpoint 1+1.5*2^-23 of floats 0x3F800001/0x3F800002.
 *        Correct rounding gives 0x3F800001. Going via double first would land on the midpoint and round to even 0x3F800002.
 *        The value just ABOVE the midpoint (...500001) must give 0x3F800002.
 */
#include "tc.h"
#include <stdlib.h>

static float F(const char *volatile s) { return strtof(s, NULL); }

TC_CASE(below_midpoint) { CHECK_EQ_U(tc_fbits(F("1.00000017881393432617187499")), 0x3F800001u); }
TC_CASE(above_midpoint) { CHECK_EQ_U(tc_fbits(F("1.00000017881393432617187501")), 0x3F800002u); }
TC_CASE(exact_midpoint_even) { CHECK_EQ_U(tc_fbits(F("1.000000178813934326171875")), 0x3F800002u); }

int main(void) { tc_begin(); TC_RUN(below_midpoint); TC_RUN(above_midpoint); TC_RUN(exact_midpoint_even); return tc_end(); }
