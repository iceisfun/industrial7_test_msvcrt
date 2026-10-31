/* TC
 * id:    scn_sscanf_hexfloat_legacy_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs scanf (pre-VS2015)
 * note:  PROBE: old scanf has no hex float: '0x1p3' %lf reads '0', stops at 'x'. Expect return 1, value 0.0, one char consumed.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;

TC_CASE(stops_at_x) { double d = 5.0; int r, n = -1; v = "0x1p3"; r = sscanf((const char *)v, "%lf%n", &d, &n);
    CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 0.0); CHECK_EQ_I(n, 1); }
int main(void) { tc_begin(); TC_RUN(stops_at_x); return tc_end(); }
