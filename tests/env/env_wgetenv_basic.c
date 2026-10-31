/* TC
 * id:    env_wgetenv_basic
 * area:  env
 * funcs: _wgetenv _wputenv
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs getenv, _wgetenv; _putenv, _wputenv
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(roundtrip)
{
    wchar_t *v;
    CHECK_EQ_I(_wputenv(L"TCV_W1=wide"), 0);
    v = _wgetenv(L"TCV_W1"); CHECK(v != NULL); if (v) CHECK_WSTR(v, L"wide");
    _wputenv(L"TCV_W1=");
}
TC_CASE(missing_null) { CHECK(_wgetenv(L"TCV_W_SURELY_MISSING_77") == NULL); }
TC_CASE(case_insensitive_name)
{
    wchar_t *v;
    _wputenv(L"TCV_WCI=Val");
    v = _wgetenv(L"tcv_wci"); CHECK(v != NULL); if (v) CHECK_WSTR(v, L"Val");
    _wputenv(L"TCV_WCI=");
}
TC_CASE(empty_name_null) { volatile const wchar_t *n = L""; CHECK(_wgetenv((const wchar_t *)n) == NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(roundtrip); TC_RUN(missing_null); TC_RUN(case_insensitive_name); TC_RUN(empty_name_null);
    return tc_end();
}
