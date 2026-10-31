/* TC
 * id:    env_neg_putenv_no_equals
 * area:  env
 * kind:  negative
 * funcs: _putenv getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _putenv (envstring must be of the form name=value)
 * note:  PROBE: return value and errno of _putenv("NAME") without '='. Expect -1 / EINVAL; the docs do not tabulate it.
 *        Handler count deliberately not asserted (handler is installed only so a fatal default cannot kill the test).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char *volatile v_bad = "TCENV_NOEQ";
static char *volatile v_nm  = "TCENV_NOEQ";

TC_CASE(return_minus1)
{
    tc_iph_install();
    CHECK_EQ_I(_putenv(v_bad), -1);
}
TC_CASE(errno_EINVAL)
{
    int r;
    tc_iph_install();
    errno = 0;
    r = _putenv(v_bad);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(variable_not_created)
{
    tc_iph_install();
    _putenv(v_bad);
    CHECK(getenv(v_nm) == NULL);
}

int main(void)
{
    tc_begin();
    TC_RUN(return_minus1);
    TC_RUN(errno_EINVAL);
    TC_RUN(variable_not_created);
    return tc_end();
}
