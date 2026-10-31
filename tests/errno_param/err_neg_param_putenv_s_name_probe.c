/* TC
 * id:    err_neg_param_putenv_s_name_probe
 * area:  errno_param
 * kind:  negative
 * funcs: _putenv_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _putenv_s (does not spell out empty name or '=' in name)
 * note:  PROBE: _putenv_s("","v") and _putenv_s("A=B","v") return EINVAL on real Windows. Handler count not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(empty_name)
{
    errno_t r; const char *volatile n = ""; tc_iph_count = 0; errno = 0;
    r = _putenv_s(n, "v");
    CHECK_EQ_I(r, EINVAL);
}
TC_CASE(name_with_equals)
{
    errno_t r; const char *volatile n = "TC_ERR_A=B"; tc_iph_count = 0; errno = 0;
    r = _putenv_s(n, "v");
    CHECK_EQ_I(r, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no handler API\n"); return 1; }
    TC_RUN(empty_name); TC_RUN(name_with_equals);
    return tc_end();
}
