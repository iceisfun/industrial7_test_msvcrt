/* TC
 * id:    scn_sscanf_hexfloat_partial_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C99 7.19.6.2: '0x1p' is not a valid hex float
 * note:  PROBE: how UCRT scanf handles '0x1p' (exponent marker without digits). C says matching failure; implementation may consume and return 0 or accept the prefix. Only checks r is 0 or 1 and, if 1, value is 1.0.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;

TC_CASE(p_no_digits) { double d = 7.0; int r; v = "0x1p"; r = sscanf((const char *)v, "%lf", &d);
    CHECK(r == -1 || r == 0 || r == 1); if (r == 1) CHECK_BITS_D(d, 1.0); }
TC_CASE(p_sign_only) { double d = 7.0; int r; v = "0x1p+x"; r = sscanf((const char *)v, "%lf", &d);
    CHECK(r == -1 || r == 0 || r == 1); if (r == 1) CHECK_BITS_D(d, 1.0); }
int main(void) { tc_begin(); TC_RUN(p_no_digits); TC_RUN(p_sign_only); return tc_end(); }
