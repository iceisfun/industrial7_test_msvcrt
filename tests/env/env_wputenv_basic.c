/* TC
 * id:    env_wputenv_basic
 * area:  env
 * funcs: _wputenv _wgetenv
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv, _wputenv; getenv, _wgetenv
 */
#include "tc.h"
#include <stdlib.h>

static wchar_t *volatile s1 = L"TCENV_WP=one";
static wchar_t *volatile s2 = L"tcenv_wp=two";
static wchar_t *volatile sd = L"TCENV_WP=";
static wchar_t *volatile nm = L"TCENV_WP";
static wchar_t *volatile nm_lo = L"tcenv_wp";

TC_CASE(add_returns_0)
{
    CHECK_EQ_I(_wputenv(s1), 0);
    CHECK(_wgetenv(nm) != NULL);
    if (_wgetenv(nm)) CHECK_WSTR(_wgetenv(nm), L"one");
    _wputenv(sd);
}
TC_CASE(replace)
{
    CHECK_EQ_I(_wputenv(s1), 0);
    CHECK_EQ_I(_wputenv(L"TCENV_WP=three"), 0);
    CHECK(_wgetenv(nm) != NULL);
    if (_wgetenv(nm)) CHECK_WSTR(_wgetenv(nm), L"three");
    _wputenv(sd);
}
TC_CASE(delete_with_empty_value)
{
    CHECK_EQ_I(_wputenv(s1), 0);
    CHECK(_wgetenv(nm) != NULL);
    CHECK_EQ_I(_wputenv(sd), 0);
    CHECK(_wgetenv(nm) == NULL);
}
TC_CASE(case_insensitive)
{
    CHECK_EQ_I(_wputenv(s1), 0);
    CHECK(_wgetenv(nm_lo) != NULL);
    CHECK_EQ_I(_wputenv(s2), 0);
    CHECK(_wgetenv(nm) != NULL);
    if (_wgetenv(nm)) CHECK_WSTR(_wgetenv(nm), L"two");
    _wputenv(sd);
}

int main(void)
{
    tc_begin();
    TC_RUN(add_returns_0); TC_RUN(replace); TC_RUN(delete_with_empty_value);
    TC_RUN(case_insensitive);
    return tc_end();
}
