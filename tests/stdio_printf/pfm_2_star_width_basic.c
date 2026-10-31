/* TC
 * id:    pfm_2_star_width_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.21.6.1p5 (negative * width = '-' flag followed by positive width); MS docs printf Width Specification
 */
#include "tc.h"
#include <stdio.h>

static volatile int w5 = 5, wm5 = -5, w3 = 3, w4 = 4;
static const char *volatile ab = "ab";

TC_CASE(two_stars_order) { char b[64]; int r = sprintf(b, "[%*d][%*d]", w3, 1, w5, 22); CHECK_STR(b, "[  1][   22]"); CHECK_EQ_I(r, 12); }
TC_CASE(neg_width_s) { char b[64]; int r = sprintf(b, "[%*s]", wm5, ab); CHECK_STR(b, "[ab   ]"); CHECK_EQ_I(r, 7); }
TC_CASE(neg_width_d) { char b[64]; sprintf(b, "[%*d]", wm5, 42); CHECK_STR(b, "[42   ]"); }
TC_CASE(star_width_and_prec) { char b[64]; sprintf(b, "[%*.*d]", w5, w3, 7); CHECK_STR(b, "[  007]"); }
TC_CASE(star_then_arg) { char b[64]; sprintf(b, "%*d|%d", w4, 9, 8); CHECK_STR(b, "   9|8"); }
TC_CASE(star_zero_width) { char b[64]; volatile int z = 0; sprintf(b, "[%*d]", z, 5); CHECK_STR(b, "[5]"); }

int main(void)
{
    tc_begin();
    TC_RUN(two_stars_order); TC_RUN(neg_width_s); TC_RUN(neg_width_d); TC_RUN(star_width_and_prec); TC_RUN(star_then_arg); TC_RUN(star_zero_width);
    return tc_end();
}
