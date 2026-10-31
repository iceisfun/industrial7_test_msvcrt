/* TC
 * id:    printf_sprintf_int_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf format specification
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(decimal)  { char b[64]; int n = sprintf(b, "%d|%i|%u", -42, 7, 4000000000u); CHECK_STR(b, "-42|7|4000000000"); CHECK_EQ_I(n, 16); }
TC_CASE(hex_oct)  { char b[64]; sprintf(b, "%x|%X|%o|%#x|%#o", 255, 255, 8, 255, 8); CHECK_STR(b, "ff|FF|10|0xff|010"); }
TC_CASE(width_flags) { char b[64]; sprintf(b, "[%5d][%-5d][%05d][%+d][% d]", 42, 42, 42, 42, 42); CHECK_STR(b, "[   42][42   ][00042][+42][ 42]"); }
TC_CASE(precision) { char b[64]; sprintf(b, "[%.5d][%8.3d][%.0d]", 42, 7, 0); CHECK_STR(b, "[00042][     007][]"); }
TC_CASE(star_width) { char b[64]; sprintf(b, "[%*d][%-*d][%.*d]", 4, 1, 4, 2, 3, 5); CHECK_STR(b, "[   1][2   ][005]"); }

int main(void) { tc_begin(); TC_RUN(decimal); TC_RUN(hex_oct); TC_RUN(width_flags); TC_RUN(precision); TC_RUN(star_width); return tc_end(); }
