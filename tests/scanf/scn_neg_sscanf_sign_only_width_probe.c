/* TC
 * id:    scn_neg_sscanf_sign_only_width_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.6.2p12, p16 (a lone sign is not a valid integer: matching failure)
 * note:  PROBES "%1d" on "-5": the width is used up by the sign, so strict C says matching failure (ret 0, dest untouched, %n not stored). Old MS CRTs may differ.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile in;
static const char *volatile fmt;

TC_CASE(sign_only_width1) { int v = 55, n = -1, r; in = "-5"; fmt = "%1d%n"; r = sscanf(in, fmt, &v, &n); CHECK_EQ_I(r, 0); CHECK_EQ_I(v, 55); CHECK_EQ_I(n, -1); }

int main(void) { tc_begin(); TC_RUN(sign_only_width1); return tc_end(); }
