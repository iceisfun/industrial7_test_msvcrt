/* TC
 * id:    scn_sscanf_hexfloat_msvcr120_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs scanf (VS2013)
 * note:  PROBE: VS2013 added C99 hex float support to some of the library; unsure for scanf. Accepts either: legacy (r==1, 0.0, 1 char consumed) or C99 (r==1, 8.0, 5 chars consumed).
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;

TC_CASE(stops_at_x) { double d = 5.0; int r, n = -1; v = "0x1p3"; r = sscanf((const char *)v, "%lf%n", &d, &n);
    CHECK_EQ_I(r, 1);
    if (n == 5) CHECK_BITS_D(d, 8.0); else { CHECK_BITS_D(d, 0.0); CHECK_EQ_I(n, 1); } }
int main(void) { tc_begin(); TC_RUN(stops_at_x); return tc_end(); }
