/* TC
 * id:    env_neg_probe_putenv_s_name_equals
 * area:  env
 * funcs: _putenv_s getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _putenv_s
 * note:  OBSERVED on Win10 22H2 ucrtbase: _putenv_s("TC_PN2=x", "v") is NOT rejected (returns 0, errno 0, no invalid-parameter
 *        call); the entry is stored as "TC_PN2=x=v" so getenv("TC_PN2") yields "x=v". The old EINVAL expectation was wrong.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(name_has_equals)
{
    int before = tc_iph_count; errno_t e;
    errno = 0; e = _putenv_s("TC_PN2=x", "v");
    CHECK_EQ_I(e, 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count - before, 0);
    CHECK_STR(getenv("TC_PN2"), "x=v");   /* observed: accepted, stored as "TC_PN2=x=v" and found by prefix */
    _putenv("TC_PN2=");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no handler API\n"); return tc_end(); }
    TC_RUN(name_has_equals);
    return tc_end();
}
