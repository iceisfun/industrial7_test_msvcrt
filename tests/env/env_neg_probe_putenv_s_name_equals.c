/* TC
 * id:    env_neg_probe_putenv_s_name_equals
 * area:  env
 * funcs: _putenv_s getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _putenv_s
 * note:  PROBE: _putenv_s with a varname containing '=' is rejected: returns EINVAL, errno EINVAL, invalid-parameter handler runs once,
 *        and the variable is not created. Believed true from the CRT's own validation but not spelled out in the docs.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(name_has_equals)
{
    int before = tc_iph_count; errno_t e;
    errno = 0; e = _putenv_s("TC_PN2=x", "v");
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - before, 1);
    CHECK(getenv("TC_PN2") == NULL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no handler API\n"); return tc_end(); }
    TC_RUN(name_has_equals);
    return tc_end();
}
