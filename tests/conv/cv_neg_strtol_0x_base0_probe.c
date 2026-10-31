/* TC
 * id:    cv_neg_strtol_0x_base0_probe
 * area:  conv
 * funcs: strtol strtoul
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.4p5 (maximal subject sequence of the expected form); see also cv_strtol_hex_prefix_only_probe
 * note:  PROBES: strtol("0x", base 0) returns 0 with endptr at the 'x' (s+1), i.e. only the '0' is consumed.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_s;

TC_CASE(zero_x_base0)
{
    static const char s[] = "0x"; char *e = 0;
    v_s = s; CHECK_EQ_I(strtol(v_s, &e, 0), 0); CHECK_EQ_P(e, s + 1);
    v_s = s; CHECK_EQ_U(strtoul(v_s, &e, 0), 0); CHECK_EQ_P(e, s + 1);
}
TC_CASE(zero_xg_base0)
{
    static const char s[] = "0xg"; char *e = 0;
    v_s = s; CHECK_EQ_I(strtol(v_s, &e, 0), 0); CHECK_EQ_P(e, s + 1);
}

int main(void) { tc_begin(); TC_RUN(zero_x_base0); TC_RUN(zero_xg_base0); return tc_end(); }
