/* TC
 * id:    cv_neg_strtol_0x_base0_probe
 * area:  conv
 * funcs: strtol strtoul
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.22.1.4p5 (maximal subject sequence of the expected form); see also cv_strtol_hex_prefix_only_probe
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): strtol/strtoul("0x", base 0) and strtol("0xg", base 0) return 0 with endptr == s
 *        (nothing consumed), not s+1. deviates from C11 7.22.1.4p4 (the "0" before a prefix-only "0x" is not converted; endptr == nptr).
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_s;

TC_CASE(zero_x_base0)
{
    static const char s[] = "0x"; char *e = 0;
    v_s = s; CHECK_EQ_I(strtol(v_s, &e, 0), 0); CHECK_EQ_P(e, s);
    v_s = s; CHECK_EQ_U(strtoul(v_s, &e, 0), 0); CHECK_EQ_P(e, s);
}
TC_CASE(zero_xg_base0)
{
    static const char s[] = "0xg"; char *e = 0;
    v_s = s; CHECK_EQ_I(strtol(v_s, &e, 0), 0); CHECK_EQ_P(e, s);
}

int main(void) { tc_begin(); TC_RUN(zero_x_base0); TC_RUN(zero_xg_base0); return tc_end(); }
