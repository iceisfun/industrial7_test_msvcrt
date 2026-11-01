/* TC
 * id:    env_neg_wputenv_s_equals_probe
 * area:  env
 * kind:  negative
 * funcs: _wputenv_s _putenv_s _wgetenv getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _putenv_s (envname must not contain '='? not stated explicitly)
 * note:  Probes that _putenv_s/_wputenv_s reject a name containing '=' with EINVAL, and that nothing is created.
 *        Win10 22H2 19045 (observed): _putenv_s/_wputenv_s do NOT reject a name containing '=': r == 0, errno untouched, no handler call; the CRT stores
 *        "NAME=B=v", so getenv("NAME") yields "B=v".
 *        [unverified] on other CRTs: only ucrtbase 19045 was observed; the expectation is gated with TC_AT_LEAST(140) and the original expectation is kept for older CRTs.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int have_iph;

TC_CASE(wide_name_has_equals)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = _wputenv_s(L"TC_WPE_A=B", L"v");

#if TC_AT_LEAST(140)
    CHECK_EQ_I(r, 0);                  /* observed: name containing '=' is accepted */
    CHECK_EQ_I(errno, 0);
    if (have_iph) CHECK_EQ_I(tc_iph_count, 0);
    CHECK_WSTR(_wgetenv(L"TC_WPE_A"), L"B=v");   /* stored as "TC_WPE_A=B=v": variable TC_WPE_A with value "B=v" */
#else
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK_EQ_I(tc_iph_count, 1);
    CHECK(_wgetenv(L"TC_WPE_A") == NULL);
#endif
    _wputenv_s(L"TC_WPE_A", L"");   /* cleanup (empty value removes the variable) */

}
TC_CASE(narrow_name_has_equals)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = _putenv_s("TC_WPE_C=D", "v");

#if TC_AT_LEAST(140)
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(errno, 0);
    CHECK_STR(getenv("TC_WPE_C"), "D=v");
#else
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(getenv("TC_WPE_C") == NULL);
#endif
    _putenv_s("TC_WPE_C", "");   /* cleanup */

}
int main(void)
{
    tc_begin();
    have_iph = tc_iph_install();
    TC_RUN(wide_name_has_equals); TC_RUN(narrow_name_has_equals);
    return tc_end();
}
