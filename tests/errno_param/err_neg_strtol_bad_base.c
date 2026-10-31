/* TC
 * id:    err_neg_strtol_bad_base
 * area:  errno_param
 * kind:  negative
 * funcs: strtol _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strtol: invalid base -> EINVAL, Parameter validation
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile int v_base;
static const char *volatile v_s = "10";

TC_CASE(base1) { char *e = NULL; long r; tc_iph_count = 0; v_base = 1; errno = 0; r = strtol(v_s, &e, v_base); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(base37) { char *e = NULL; long r; tc_iph_count = 0; v_base = 37; errno = 0; r = strtol(v_s, &e, v_base); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(handler) { char *e = NULL; tc_iph_count = 0; v_base = 1; strtol(v_s, &e, v_base); CHECK(tc_iph_count >= 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(base1); TC_RUN(base37); TC_RUN(handler);
    return tc_end();
}
