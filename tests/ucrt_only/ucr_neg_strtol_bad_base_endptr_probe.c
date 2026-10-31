/* TC
 * id:    ucr_neg_strtol_bad_base_endptr_probe
 * area:  ucrt_only
 * funcs: strtol strtoul
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtol (no conversion: endptr set to nptr)
 * note:  PROBE: with an unsupported base (1, 37) strtol/strtoul still store nptr in *endptr before the parameter validation fails.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_s;
static volatile int v_base;

TC_CASE(strtol_endptr)
{
    char *e = NULL; v_s = "123"; v_base = 37; tc_iph_count = 0;
    CHECK_EQ_I(strtol(v_s, &e, v_base), 0);
    CHECK_EQ_P(e, v_s);
}
TC_CASE(strtoul_endptr)
{
    char *e = NULL; v_s = "123"; v_base = 1; tc_iph_count = 0;
    CHECK_EQ_U(strtoul(v_s, &e, v_base), 0);
    CHECK_EQ_P(e, v_s);
}
int main(void) { tc_begin(); if (!tc_iph_install()) return 2; TC_RUN(strtol_endptr); TC_RUN(strtoul_endptr); return tc_end(); }
