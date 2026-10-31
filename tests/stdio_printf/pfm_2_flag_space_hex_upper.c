/* TC
 * id:    pfm_2_flag_space_hex_upper
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.1p6 (space flag: signed conversions only); MS docs printf flags
 * note:  PROBE: sign flags on unsigned conversions (x X o u) are not specified by C11 (flags apply to signed conversions); MS docs describe + and space for signed output. Expected: flag ignored.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static volatile unsigned ff = 255, eight = 8, seven = 7;

TC_CASE(hexu) { char b[32]; sprintf(b, "% X", ff); CHECK_STR(b, "FF"); }
TC_CASE(oct) { char b[32]; sprintf(b, "% o", eight); CHECK_STR(b, "10"); }
TC_CASE(uns) { char b[32]; sprintf(b, "% u", seven); CHECK_STR(b, "7"); }
TC_CASE(width) { char b[32]; sprintf(b, "% 5X", ff); CHECK_STR(b, "   FF"); }

int main(void) { tc_begin(); TC_RUN(hexu); TC_RUN(oct); TC_RUN(uns); TC_RUN(width); return tc_end(); }
