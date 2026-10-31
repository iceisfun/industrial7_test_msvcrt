/* TC
 * id:    env_neg_putenv_args
 * area:  env
 * funcs: _putenv _putenv_s _wputenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _putenv, _putenv_s, _wputenv_s (NULL arguments are invalid parameters)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(putenv_s_name_NULL)
{
    int before = tc_iph_count; errno_t e;
    errno = 0; e = _putenv_s(NULL, "v");
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - before, 1);
}
TC_CASE(putenv_s_value_NULL)
{
    int before = tc_iph_count; errno_t e;
    errno = 0; e = _putenv_s("TC_PN1", NULL);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - before, 1);
}
TC_CASE(wputenv_s_name_NULL)
{
    int before = tc_iph_count; errno_t e;
    errno = 0; e = _wputenv_s(NULL, L"v");
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - before, 1);
}
TC_CASE(putenv_NULL)
{
    int before = tc_iph_count; int r;
    errno = 0; r = _putenv(NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - before, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no handler API\n"); return tc_end(); }
    TC_RUN(putenv_s_name_NULL); TC_RUN(putenv_s_value_NULL);
    TC_RUN(wputenv_s_name_NULL); TC_RUN(putenv_NULL);
    return tc_end();
}
