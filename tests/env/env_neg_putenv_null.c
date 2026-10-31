/* TC
 * id:    env_neg_putenv_null
 * area:  env
 * kind:  negative
 * funcs: _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv / _wputenv: envstring NULL invokes the invalid parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char *volatile v_null = NULL;

TC_CASE(returns_minus1)
{
    tc_iph_install();
    CHECK_EQ_I(_putenv(v_null), -1);
}
TC_CASE(errno_EINVAL)
{
    tc_iph_install();
    errno = 0;
    CHECK_EQ_I(_putenv(v_null), -1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(iph_count_1)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    _putenv(v_null);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_RUN(returns_minus1);
    TC_RUN(errno_EINVAL);
    TC_RUN(iph_count_1);
    return tc_end();
}
