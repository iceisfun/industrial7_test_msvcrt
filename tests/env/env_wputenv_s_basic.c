/* TC
 * id:    env_wputenv_s_basic
 * area:  env
 * funcs: _wputenv_s _wgetenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv_s, _wputenv_s (empty value_string removes the variable; returns 0 on success)
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(add)
{
    wchar_t *v;
    CHECK(_wgetenv(L"TC_WPS_ADD") == NULL);
    CHECK_EQ_I(_wputenv_s(L"TC_WPS_ADD", L"one"), 0);
    v = _wgetenv(L"TC_WPS_ADD");
    CHECK(v != NULL);
    if (v) CHECK_WSTR(v, L"one");
    _wputenv_s(L"TC_WPS_ADD", L"");
}
TC_CASE(replace)
{
    wchar_t *v;
    CHECK_EQ_I(_wputenv_s(L"TC_WPS_REP", L"first"), 0);
    CHECK_EQ_I(_wputenv_s(L"TC_WPS_REP", L"second-longer"), 0);
    v = _wgetenv(L"TC_WPS_REP");
    CHECK(v != NULL);
    if (v) CHECK_WSTR(v, L"second-longer");
    CHECK_EQ_I(_wputenv_s(L"TC_WPS_REP", L"x"), 0);
    v = _wgetenv(L"TC_WPS_REP");
    if (v) CHECK_WSTR(v, L"x");
    _wputenv_s(L"TC_WPS_REP", L"");
}
TC_CASE(empty_deletes)
{
    CHECK_EQ_I(_wputenv_s(L"TC_WPS_DEL", L"gone"), 0);
    CHECK(_wgetenv(L"TC_WPS_DEL") != NULL);
    CHECK_EQ_I(_wputenv_s(L"TC_WPS_DEL", L""), 0);
    CHECK(_wgetenv(L"TC_WPS_DEL") == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(add); TC_RUN(replace); TC_RUN(empty_deletes);
    return tc_end();
}
