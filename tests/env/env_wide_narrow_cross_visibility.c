/* TC
 * id:    env_wide_narrow_cross_visibility
 * area:  env
 * funcs: _putenv _wputenv getenv _wgetenv
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv/_wputenv: when both the MBCS and the Unicode environment copies exist the CRT keeps them in sync; getenv is not case sensitive on Windows
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(putenv_then_wgetenv)
{
    wchar_t *w;
    CHECK_EQ_I(_putenv("TC_XV_A=narrowset"), 0);
    w = _wgetenv(L"TC_XV_A");
    CHECK(w != NULL);
    if (w) CHECK_WSTR(w, L"narrowset");
    _putenv("TC_XV_A=");
}
TC_CASE(wputenv_then_getenv)
{
    char *n;
    (void)_wgetenv(L"PATH");  /* make sure the wide environment exists */
    CHECK_EQ_I(_wputenv(L"TC_XV_B=wideset"), 0);
    n = getenv("TC_XV_B");
    CHECK(n != NULL);
    if (n) CHECK_STR(n, "wideset");
    _wputenv(L"TC_XV_B=");
}
TC_CASE(narrow_delete_seen_by_wide)
{
    CHECK_EQ_I(_putenv("TC_XV_C=1"), 0);
    CHECK(_wgetenv(L"TC_XV_C") != NULL);
    CHECK_EQ_I(_putenv("TC_XV_C="), 0);
    CHECK(_wgetenv(L"TC_XV_C") == NULL);
}
TC_CASE(wide_delete_seen_by_narrow)
{
    CHECK_EQ_I(_wputenv(L"TC_XV_D=1"), 0);
    CHECK(getenv("TC_XV_D") != NULL);
    CHECK_EQ_I(_wputenv(L"TC_XV_D="), 0);
    CHECK(getenv("TC_XV_D") == NULL);
}
TC_CASE(case_insensitive_cross)
{
    wchar_t *w; char *n;
    CHECK_EQ_I(_putenv("TcXvE=MixedCase"), 0);
    w = _wgetenv(L"TCXVE");
    CHECK(w != NULL);
    if (w) CHECK_WSTR(w, L"MixedCase");
    n = getenv("tcxve");
    CHECK(n != NULL);
    if (n) CHECK_STR(n, "MixedCase");
    CHECK_EQ_I(_wputenv(L"tcxve="), 0);   /* delete through a different case */
    CHECK(getenv("TcXvE") == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(putenv_then_wgetenv); TC_RUN(wputenv_then_getenv); TC_RUN(narrow_delete_seen_by_wide);
    TC_RUN(wide_delete_seen_by_narrow); TC_RUN(case_insensitive_cross);
    return tc_end();
}
