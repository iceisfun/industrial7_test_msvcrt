/* TC
 * id:    pfi_int_minmax_char_short
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p7, 6.3.1.1; MS docs printf
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(hd) { char b[64]; volatile short mn = SHRT_MIN, mx = SHRT_MAX; sprintf(b, "%hd|%hd", mn, mx); CHECK_STR(b, "-32768|32767"); }
TC_CASE(hu) { char b[64]; volatile unsigned short u = USHRT_MAX; sprintf(b, "%hu", u); CHECK_STR(b, "65535"); }
TC_CASE(promoted_x) { char b[64]; volatile short s = -1; sprintf(b, "%x", s); CHECK_STR(b, "ffffffff"); }
TC_CASE(hx) { char b[64]; volatile short s = -1; sprintf(b, "%hx|%ho", s, s); CHECK_STR(b, "ffff|177777"); }
TC_CASE(char_d) { char b[64]; volatile signed char c = -1; volatile unsigned char uc = 255; sprintf(b, "%d|%d", c, uc); CHECK_STR(b, "-1|255"); }

int main(void) { tc_begin(); TC_RUN(hd); TC_RUN(hu); TC_RUN(promoted_x); TC_RUN(hx); TC_RUN(char_d); return tc_end(); }
