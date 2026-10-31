/* TC
 * id:    env_neg_wputenv_s_equals_probe
 * area:  env
 * kind:  negative
 * funcs: _wputenv_s _putenv_s _wgetenv getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _putenv_s (envname must not contain '='? not stated explicitly)
 * note:  Probes that _putenv_s/_wputenv_s reject a name containing '=' with EINVAL, and that nothing is created.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int have_iph;

TC_CASE(wide_name_has_equals)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = _wputenv_s(L"TC_WPE_A=B", L"v");
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK_EQ_I(tc_iph_count, 1);
    CHECK(_wgetenv(L"TC_WPE_A") == NULL);
}
TC_CASE(narrow_name_has_equals)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = _putenv_s("TC_WPE_C=D", "v");
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(getenv("TC_WPE_C") == NULL);
}
int main(void)
{
    tc_begin();
    have_iph = tc_iph_install();
    TC_RUN(wide_name_has_equals); TC_RUN(narrow_name_has_equals);
    return tc_end();
}
