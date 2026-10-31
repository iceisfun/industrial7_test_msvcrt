/* TC
 * id:    cv_strtol_base2_0b_probe
 * area:  conv
 * funcs: strtol

 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   C23 7.24.1.7 adds 0b for base 0/2; C11 and Microsoft docs do not
 * note:  PROBE: whether strtol("0b101", base 2) recognises the 0b prefix. Expected (most likely, pre-C23 runtime): not recognised, value 0, endptr at 'b'.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile p = "0b101";

TC_CASE(base2_0b) { char *e; CHECK_EQ_I(strtol(p, &e, 2), 0); CHECK_EQ_P(e, p + 1); }
TC_CASE(base0_0b) { char *e; CHECK_EQ_I(strtol(p, &e, 0), 0); CHECK_EQ_P(e, p + 1); }

int main(void) { tc_begin(); TC_RUN(base2_0b); TC_RUN(base0_0b); return tc_end(); }
