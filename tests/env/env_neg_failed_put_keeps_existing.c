/* TC
 * id:    env_neg_failed_put_keeps_existing
 * area:  env
 * kind:  negative
 * funcs: _putenv _putenv_s _wputenv_s getenv _wgetenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv_s (NULL name/value -> EINVAL, handler invoked), _putenv (NULL -> -1)
 * note:  A failed call must not modify or delete a pre-existing variable.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile nullp = NULL;
static const wchar_t *volatile wnullp = NULL;

static void setup(void) { _putenv("TC_KEEP1=orig"); }

TC_CASE(putenv_s_value_null_keeps_old)
{
    int b; errno_t e; setup(); b = tc_iph_count; errno = 0;
    e = _putenv_s("TC_KEEP1", nullp);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - b, 1);
    CHECK_STR(getenv("TC_KEEP1"), "orig");
}
TC_CASE(wputenv_s_value_null_keeps_old)
{
    int b; errno_t e; setup(); b = tc_iph_count; errno = 0;
    e = _wputenv_s(L"TC_KEEP1", wnullp);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - b, 1);
    CHECK(_wgetenv(L"TC_KEEP1") != NULL);
    if (_wgetenv(L"TC_KEEP1")) CHECK_WSTR(_wgetenv(L"TC_KEEP1"), L"orig");
}
TC_CASE(putenv_null_keeps_old)
{
    char buf[16]; DWORD n; int b, r; setup(); b = tc_iph_count;
    r = _putenv(nullp);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count - b, 1);
    CHECK_STR(getenv("TC_KEEP1"), "orig");
    n = GetEnvironmentVariableA("TC_KEEP1", buf, sizeof buf);
    CHECK_EQ_U(n, 4);
    CHECK_STR(buf, "orig");
}
TC_CASE(putenv_s_name_null_keeps_old)
{
    int b; errno_t e; setup(); b = tc_iph_count;
    e = _putenv_s(nullp, "x");
    CHECK_EQ_I(e, EINVAL);
    CHECK_EQ_I(tc_iph_count - b, 1);
    CHECK_STR(getenv("TC_KEEP1"), "orig");
}
TC_CASE(handler_count_each_is_1)
{
    int b; setup();
    b = tc_iph_count; _putenv_s("TC_KEEP1", nullp); CHECK_EQ_I(tc_iph_count - b, 1);
    b = tc_iph_count; _putenv(nullp);               CHECK_EQ_I(tc_iph_count - b, 1);
    b = tc_iph_count; _wputenv_s(L"TC_KEEP1", wnullp); CHECK_EQ_I(tc_iph_count - b, 1);
    b = tc_iph_count; _putenv_s(nullp, "x");        CHECK_EQ_I(tc_iph_count - b, 1);
    CHECK_STR(getenv("TC_KEEP1"), "orig");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(putenv_s_value_null_keeps_old); TC_RUN(wputenv_s_value_null_keeps_old);
    TC_RUN(putenv_null_keeps_old); TC_RUN(putenv_s_name_null_keeps_old); TC_RUN(handler_count_each_is_1);
    _putenv("TC_KEEP1=");
    return tc_end();
}
