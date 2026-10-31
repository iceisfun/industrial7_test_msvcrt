/* TC
 * id:    env_neg_wgetenv_s_errors
 * area:  env
 * funcs: _wgetenv_s _wputenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs getenv_s, _wgetenv_s
 * note:  Handler counts asserted only where _set_invalid_parameter_handler exists.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int have_iph;
static volatile size_t vsz = 8;

TC_CASE(too_small_ERANGE_and_required_size)
{
    size_t ret = 0; wchar_t buf[4]; errno_t e;
    _wputenv(L"TC_WE1=0123456789");
    e = _wgetenv_s(&ret, buf, 4, L"TC_WE1");
    CHECK_EQ_I(e, ERANGE);
    CHECK_EQ_U(ret, 11);
}
TC_CASE(buffer_NULL_size_gt0_EINVAL)
{
    size_t ret = 5; int before = tc_iph_count; errno_t e;
    errno = 0; e = _wgetenv_s(&ret, NULL, vsz, L"TC_WE1");
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK_EQ_I(tc_iph_count - before, 1);
}
TC_CASE(varname_NULL_EINVAL)
{
    size_t ret = 5; wchar_t buf[8]; int before = tc_iph_count; errno_t e;
    errno = 0; e = _wgetenv_s(&ret, buf, 8, NULL);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK_EQ_I(tc_iph_count - before, 1);
}
TC_CASE(pReturnValue_NULL_EINVAL)
{
    wchar_t buf[8]; int before = tc_iph_count; errno_t e;
    errno = 0; e = _wgetenv_s(NULL, buf, 8, L"TC_WE1");
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK_EQ_I(tc_iph_count - before, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(too_small_ERANGE_and_required_size); TC_RUN(buffer_NULL_size_gt0_EINVAL);
    TC_RUN(varname_NULL_EINVAL); TC_RUN(pReturnValue_NULL_EINVAL);
    return tc_end();
}
