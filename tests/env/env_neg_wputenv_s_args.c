/* TC
 * id:    env_neg_wputenv_s_args
 * area:  env
 * kind:  negative
 * funcs: _wputenv_s _wgetenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv_s, _wputenv_s (NULL envname or value_string -> EINVAL return and errno, invalid parameter handler)
 * note:  The '=' in name case is probed separately (env_neg_wputenv_s_equals_probe). msvcrt.dll is excluded: it may lack
 *        _set_invalid_parameter_handler and its default reaction to a bad parameter is not known.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int have_iph;

TC_CASE(name_null)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = _wputenv_s(NULL, L"v");
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(value_null)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = _wputenv_s(L"TC_WPSN_V", NULL);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK_EQ_I(tc_iph_count, 1);
    CHECK(_wgetenv(L"TC_WPSN_V") == NULL);
}
TC_CASE(both_null)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = _wputenv_s(NULL, NULL);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin();
    have_iph = tc_iph_install();
    TC_RUN(name_null); TC_RUN(value_null); TC_RUN(both_null);
    return tc_end();
}
