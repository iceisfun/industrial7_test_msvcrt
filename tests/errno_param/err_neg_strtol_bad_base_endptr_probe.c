/* TC
 * id:    err_neg_strtol_bad_base_endptr_probe
 * area:  errno_param
 * kind:  negative
 * funcs: strtol _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strtol (silent on endptr for invalid base)
 * note:  PROBE: endptr is set to the start of the string when the base is invalid (UCRT does *endptr = nptr before validating).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile int v_base;
static const char *volatile v_s = "10";

TC_CASE(endptr_start) { char *e = (char *)1; const char *s = v_s; v_base = 1; strtol(s, &e, v_base); CHECK_EQ_P(e, s); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(endptr_start);
    return tc_end();
}
