/* TC
 * id:    scn_sscanf_hex_noprefix_digits_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.2 (strtoul-like subject sequence)
 * note:  PROBE: '0x' with no hex digits after it. Assumes the MS CRT counts the leading 0 as a digit, consumes the x, and returns 1 with value 0 and 2 chars consumed.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(x_only) { unsigned a = 5; int n, pos = -1; v = "0x"; n = sscanf((const char *)v, "%x%n", &a, &pos); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, 0); CHECK_EQ_I(pos, 2); }
TC_CASE(xg) { unsigned a = 5; int n, pos = -1; v = "0xg"; n = sscanf((const char *)v, "%x%n", &a, &pos); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, 0); CHECK_EQ_I(pos, 2); }
TC_CASE(i_x_only) { int a = 5; int n, pos = -1; v = "0x"; n = sscanf((const char *)v, "%i%n", &a, &pos); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 0); CHECK_EQ_I(pos, 2); }
int main(void) { tc_begin(); TC_RUN(x_only); TC_RUN(xg); TC_RUN(i_x_only); return tc_end(); }
