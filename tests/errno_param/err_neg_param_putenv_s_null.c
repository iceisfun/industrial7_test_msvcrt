/* TC
 * id:    err_neg_param_putenv_s_null
 * area:  errno_param
 * kind:  negative
 * funcs: _putenv_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _putenv_s, _wputenv_s
 * note:  NULL name/value only; empty-name and '=' cases are in err_neg_param_putenv_s_name_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(null_name)
{
    errno_t r; const char *volatile n = NULL; tc_iph_count = 0; errno = 0;
    r = _putenv_s(n, "v");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_value)
{
    errno_t r; const char *volatile v = NULL; tc_iph_count = 0; errno = 0;
    r = _putenv_s("TC_ERR_PUTENV_NULLVAL", v);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no handler API\n"); return 1; }
    TC_RUN(null_name); TC_RUN(null_value);
    return tc_end();
}
