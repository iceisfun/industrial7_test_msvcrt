/* TC
 * id:    err_neg_fclose_null
 * area:  errno_param
 * kind:  negative
 * funcs: fclose _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fclose, Parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static FILE *volatile v_null;

TC_CASE(returns_eof) { int r; tc_iph_count = 0; errno = 0; r = fclose(v_null); CHECK_EQ_I(r, EOF); }
TC_CASE(sets_einval) { tc_iph_count = 0; errno = 0; fclose(v_null); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(handler_once) { tc_iph_count = 0; fclose(v_null); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(returns_eof); TC_RUN(sets_einval); TC_RUN(handler_once);
    return tc_end();
}
