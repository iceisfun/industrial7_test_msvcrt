/* TC
 * id:    err_neg_param_putenv_s_name_probe
 * area:  errno_param
 * kind:  negative
 * funcs: _putenv_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _putenv_s (does not spell out empty name or '=' in name)
 * note:  OBSERVED on Win10 22H2 (19045): _putenv_s("","v") returns EINVAL, but _putenv_s("TC_ERR_A=B","v") returns 0 (no
 *        handler call) and creates variable TC_ERR_A with value "B=v". The earlier EINVAL expectation for a name containing '='
 *        was wrong. Stable.
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
    CHECK_EQ_I(r, 0);                      /* observed: accepted; the CRT splits at the first '=' only on its own, so name TC_ERR_A gets value "B=v" */
    CHECK_STR(getenv("TC_ERR_A"), "B=v");
    _putenv_s("TC_ERR_A", "");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no handler API\n"); return 1; }
    TC_RUN(empty_name); TC_RUN(name_with_equals);
    return tc_end();
}
