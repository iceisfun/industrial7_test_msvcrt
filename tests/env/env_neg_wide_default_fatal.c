/* TC
 * id:    env_neg_wide_default_fatal
 * area:  env
 * kind:  negative
 * funcs: _wgetenv _wgetenv_s _wputenv _wputenv_s _wdupenv_s getenv _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs Parameter validation; _wgetenv_s, _wputenv_s, _wdupenv_s: NULL argument invokes the invalid parameter handler, which terminates by default
 * note:  Children run with the default handler (tc_iph_default() at child start). Terminating cases assert abnormal termination (not
 *        77 = returned, not 0). OBSERVED on Win10 22H2 ucrtbase: _wgetenv_s with a NULL varname is NOT validated (the child
 *        returns normally, exit 77), like getenv_s; the other four NULL cases terminate.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

static int child_main(const char *c)
{
    volatile const wchar_t *wn = NULL; wchar_t buf[8]; size_t r = 0; wchar_t *dup = NULL; size_t n = 0;
    volatile wchar_t **vb = NULL;
    tc_iph_default();
    if (tc_strequ(c, "wgetenv")) (void)_wgetenv((const wchar_t *)wn);
    else if (tc_strequ(c, "wgetenv_s")) (void)_wgetenv_s(&r, buf, 8, (const wchar_t *)wn);
    else if (tc_strequ(c, "wputenv")) (void)_wputenv((const wchar_t *)wn);
    else if (tc_strequ(c, "wputenv_s")) (void)_wputenv_s(L"TC_WF", (const wchar_t *)wn);
    else if (tc_strequ(c, "wdupenv_s")) (void)_wdupenv_s((wchar_t **)vb, &n, L"PATH");
    (void)dup;
    return 77;
}
static void run_ret(const char *name)   /* expects the child to return normally (77) */
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);
}
static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
}
TC_CASE(child_wgetenv_null) { run("wgetenv"); }
TC_CASE(child_wgetenv_s_varname_null) { run_ret("wgetenv_s"); }   /* observed: NULL varname is not validated */
TC_CASE(child_wputenv_null) { run("wputenv"); }
TC_CASE(child_wputenv_s_value_null) { run("wputenv_s"); }
TC_CASE(child_wdupenv_s_buffer_null) { run("wdupenv_s"); }
TC_CASE(parent_not_hung_and_env_unmodified)
{
    _putenv("TC_WF=parent");
    run("wputenv_s");
    CHECK_STR(getenv("TC_WF"), "parent");
    CHECK(_wgetenv(L"TC_WF") != NULL);
    _putenv("TC_WF=");
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(child_wgetenv_null); TC_RUN(child_wgetenv_s_varname_null); TC_RUN(child_wputenv_null);
    TC_RUN(child_wputenv_s_value_null); TC_RUN(child_wdupenv_s_buffer_null);
    TC_RUN(parent_not_hung_and_env_unmodified);
    return tc_end();
}
