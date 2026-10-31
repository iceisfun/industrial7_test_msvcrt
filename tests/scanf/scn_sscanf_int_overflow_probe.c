/* TC
 * id:    scn_sscanf_int_overflow_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.2p10 (UB on overflow); MS CRT accumulates modulo 2^N
 * note:  PROBE: out-of-range integer text wraps modularly rather than saturating (UB per C standard).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(d_2p31) { int a = 5; int n; v = "2147483648"; n = sscanf((const char *)v, "%d", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, INT_MIN); }
TC_CASE(d_2p32) { int a = 5; int n; v = "4294967296"; n = sscanf((const char *)v, "%d", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 0); }
TC_CASE(u_2p32) { unsigned a = 5; int n; v = "4294967296"; n = sscanf((const char *)v, "%u", &a); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, 0); }
TC_CASE(hd_65536) { short a = 5; int n; v = "65536"; n = sscanf((const char *)v, "%hd", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 0); }
int main(void) { tc_begin(); TC_RUN(d_2p31); TC_RUN(d_2p32); TC_RUN(u_2p32); TC_RUN(hd_65536); return tc_end(); }
