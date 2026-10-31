/* TC
 * id:    env_neg_wputenv_args
 * area:  env
 * kind:  negative
 * funcs: _wputenv _wgetenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv, _wputenv (envstring NULL -> invalid parameter handler, errno EINVAL, return -1)
 * note:  Only the NULL argument is asserted here. Malformed strings (no '=', empty, leading '=') are probed in
 *        env_neg_wputenv_noequals_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int have_iph;

TC_CASE(null_arg)
{
    int r; tc_iph_count = 0; errno = 0;
    r = _wputenv(NULL);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_leaves_env_intact)
{
    wchar_t *v;
    CHECK_EQ_I(_wputenv(L"TC_NWA_KEEP=kept"), 0);
    errno = 0; _wputenv(NULL);
    v = _wgetenv(L"TC_NWA_KEEP");
    CHECK(v != NULL);
    if (v) CHECK_WSTR(v, L"kept");
    CHECK_EQ_I(_wputenv(L"TC_NWA_KEEP="), 0);
    CHECK(_wgetenv(L"TC_NWA_KEEP") == NULL);
}
int main(void)
{
    tc_begin();
    have_iph = tc_iph_install();
    TC_RUN(null_arg); TC_RUN(null_leaves_env_intact);
    return tc_end();
}
