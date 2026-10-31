/* TC
 * id:    ucr_p_environ_putenv
 * area:  ucrt_only
 * funcs: __p__environ _putenv_s _wputenv_s getenv
 * procs: __p__wenviron _wgetenv
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv_s, _wputenv_s, _environ, _wenviron ("remove a variable by specifying an empty string for value")
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

typedef wchar_t ***(__cdecl *pwenv_t)(void);
typedef wchar_t *(__cdecl *wgetenv_t)(const wchar_t *);
static pwenv_t pwenv; static wgetenv_t wgetenv_;

static int has_entry(const char *s)
{ char **e = *__p__environ(); int i; if (!e) return 0; for (i = 0; e[i]; i++) if (tc_strequ(e[i], s)) return 1; return 0; }
static int has_wentry(const wchar_t *s)
{ wchar_t **e = *pwenv(); int i; if (!e) return 0; for (i = 0; e[i]; i++) if (tc_wcsequ(e[i], s)) return 1; return 0; }

TC_CASE(add)
{
    CHECK_EQ_I(_putenv_s("TC_UCR_PE1", "one"), 0);
    CHECK_STR(getenv("TC_UCR_PE1"), "one");
    CHECK(has_entry("TC_UCR_PE1=one"));
    _putenv_s("TC_UCR_PE1", "");
}
TC_CASE(replace)
{
    CHECK_EQ_I(_putenv_s("TC_UCR_PE2", "a"), 0);
    CHECK_EQ_I(_putenv_s("TC_UCR_PE2", "bb"), 0);
    CHECK_STR(getenv("TC_UCR_PE2"), "bb");
    CHECK(has_entry("TC_UCR_PE2=bb"));
    CHECK(!has_entry("TC_UCR_PE2=a"));
    _putenv_s("TC_UCR_PE2", "");
}
TC_CASE(empty_deletes)
{
    CHECK_EQ_I(_putenv_s("TC_UCR_PE3", "x"), 0);
    CHECK(getenv("TC_UCR_PE3") != NULL);
    CHECK_EQ_I(_putenv_s("TC_UCR_PE3", ""), 0);
    CHECK(getenv("TC_UCR_PE3") == NULL);
    CHECK(!has_entry("TC_UCR_PE3=x"));
}
TC_CASE(wide)
{
    if (!pwenv || !wgetenv_) TC_SKIP("no wide env exports");
    CHECK_EQ_I(_wputenv_s(L"TC_UCR_PW1", L"wide"), 0);
    CHECK_WSTR(wgetenv_(L"TC_UCR_PW1"), L"wide");
    CHECK(*pwenv() != NULL);
    CHECK(has_wentry(L"TC_UCR_PW1=wide"));
    CHECK_EQ_I(_wputenv_s(L"TC_UCR_PW1", L""), 0);
    CHECK(wgetenv_(L"TC_UCR_PW1") == NULL);
    CHECK(!has_wentry(L"TC_UCR_PW1=wide"));
}
int main(void)
{
    tc_begin();
    pwenv = (pwenv_t)tc_crt_proc("__p__wenviron");
    wgetenv_ = (wgetenv_t)tc_crt_proc("_wgetenv");
    TC_RUN(add); TC_RUN(replace); TC_RUN(empty_deletes); TC_RUN(wide);
    return tc_end();
}
